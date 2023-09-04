#include "action/ProtectedPath.h"
#include "action/SequentialFilling.h"
#include "destination/DestinationDirectory.h"
#include "destination/LoggedDestination.h"
#include "file-repository/FileCachedFileRepository.h"
#include "file-repository/OverloadedFileRepository.h"
#include "file/FakeFile.h"
#include "http/HttpClient.h"
#include "http/LoggedHttp.h"
#include "http/MemCachedHttp.h"
#include "log/FileLog.h"
#include "log/ForkedLog.h"
#include "log/StreamLog.h"
#include "log/SynchronizedLog.h"
#include "package/MemPackage.h"
#include "profile/JsonProfile.h"
#include "re146/FileRepository.h"
#include "re146/Repository.h"
#include "repository/MemRepository.h"
#include "repository/OverloadedRepository.h"
#include "scalar/JsonFromFile.h"
#include "solution/FileCachedSolution.h"
#include "solution/PubgrubSolution.h"

// NOLINTNEXTLINE(*-no-recursion)
auto stringify(const std::exception &e, int level) -> std::string {
    auto part = std::string(level, ' ') + "exception: " + e.what() + '\n';
    try {
        std::rethrow_if_nested(e);
        return part;
    } catch (const std::exception &nestedException) {
        return part + stringify(nestedException, level + 1);
    }
}

void conditionalThrow(bool condition, const std::string &message) {
    if (not condition) throw std::runtime_error(message);
}

void set_locale(const char *locale) {
    (void)std::setlocale(LC_ALL, locale); // NOLINT(*-mt-unsafe)
}

auto get_locale() -> std::string_view {
    return std::setlocale(LC_ALL, nullptr); // NOLINT(*-mt-unsafe)
}

auto main(int argc, char **argv) -> int {
    auto args = std::span(argv, size_t(argc));

    auto rootFolder = std::filesystem::path("fabgrub");
    auto profilesFolder = rootFolder / "profiles";

    auto log = make<SynchronizedLog>(make<ForkedLog>(
        make<StreamLog>(std::cout),
        make<FileLog>(rootFolder / "log.txt")
    ));

    try {
        set_locale("");
        log->info("Current locale: {}", get_locale());

        conditionalThrow(
            args.size() == 2,
            fmt::format(
                "FabGrub expect one console argument, but got {}",
                args.size() - 1
            )
        );

        auto profileName = std::string_view(args[1]);
        auto profileFile = profilesFolder / fmt::format("{}.json", profileName);
        auto profile = make<JsonProfile>(make<JsonFromFile>(profileFile));
        log->info("Chosen profile: {}", profileName);
        log->info(
            "Chosen Factorio version: {}",
            fmt::streamed(*profile->factorioVersion())
        );

        auto basePackage = make<MemPackage>("base", profile->factorioVersion());
        auto http = make<LoggedHttp>(log, make<HttpClient>());
        auto modsFolder = std::filesystem::path("mods");
        auto app = make<ProtectedPath>(
            modsFolder,
            rootFolder / "mods-original",
            make<SequentialFilling>(
                make<LoggedDestination>(
                    log,
                    make<DestinationDirectory>(
                        modsFolder,
                        make<OverloadedFileRepository>(
                            basePackage,
                            make<FakeFile>(),
                            make<FileCachedFileRepository>(
                                rootFolder / "mods",
                                make<re146::FileRepository>(http)
                            )
                        )
                    )
                ),
                make<FileCachedSolution>(
                    std::filesystem::last_write_time(profileFile),
                    profilesFolder / fmt::format("{}.lock.json", profileName),
                    make<PubgrubSolution>(
                        profile->requirements(),
                        make<OverloadedRepository>(
                            basePackage->name(),
                            make<MemPackages>(basePackage),
                            make<re146::Repository>(make<MemCachedHttp>(http))
                        )
                    )
                )
            )
        );

        (*app)();
    } catch (const std::exception &e) {
        log->info("{}", stringify(e, 0));
        return 1;
    }
}
