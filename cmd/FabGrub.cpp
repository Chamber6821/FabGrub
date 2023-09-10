#include "action/Executable.h"
#include "action/ProtectedPath.h"
#include "action/Sequence.h"
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
#include <span>

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
    try {
        try {
            auto args = std::span(argv, static_cast<size_t>(argc));

            auto root = std::filesystem::current_path();
            auto home = root / "fabgrub";
            auto profiles = home / "profiles";

            auto log = make<SynchronizedLog>(make<ForkedLog>(
                make<StreamLog>(std::cout),
                make<FileLog>(home / "log.txt")
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
                auto profileFile =
                    profiles / fmt::format("{}.json", profileName);
                auto profile =
                    make<JsonProfile>(make<JsonFromFile>(profileFile));
                log->info("Chosen profile: {}", profileName);
                log->info(
                    "Chosen Factorio version: {}",
                    fmt::streamed(*profile->factorioVersion())
                );

                auto basePackage =
                    make<MemPackage>("base", profile->factorioVersion());
                auto http = make<LoggedHttp>(log, make<HttpClient>());
                auto mods = root / "mods";
                auto app = make<ProtectedPath>(
                    mods,
                    home / "mods-original",
                    log,
                    make<ProtectedPath>(
                        profiles / fmt::format("{}.settings.dat", profileName),
                        mods / "mod-settings.dat",
                        log,
                        make<Sequence>(
                            make<SequentialFilling>(
                                make<LoggedDestination>(
                                    log,
                                    make<DestinationDirectory>(
                                        mods,
                                        make<OverloadedFileRepository>(
                                            basePackage,
                                            make<FakeFile>(),
                                            make<FileCachedFileRepository>(
                                                home / "mods",
                                                make<re146::FileRepository>(http
                                                )
                                            )
                                        )
                                    )
                                ),
                                make<FileCachedSolution>(
                                    std::filesystem::last_write_time(profileFile
                                    ),
                                    profiles / fmt::format(
                                                   "{}.lock.json",
                                                   profileName
                                               ),
                                    make<PubgrubSolution>(
                                        profile->requirements(),
                                        make<OverloadedRepository>(
                                            basePackage->name(),
                                            make<MemPackages>(basePackage),
                                            make<re146::Repository>(
                                                make<MemCachedHttp>(http)
                                            )
                                        )
                                    )
                                )
                            ),
                            make<Executable>(root / "bin" / "x64" / "factorio")
                        )
                    )
                );

                (*app)();
            } catch (const std::exception &e) {
                log->info("{}", stringify(e, 0));
                return 1;
            }
        } catch (const std::exception &e) {
            std::cerr << "Unexpected std::exception: " << e.what() << '\n';
            return 2;
        } catch (...) {
            std::cerr << "Unexpected custom exception\n";
            return 3;
        }
    } catch (...) {
        return 4;
    }
}
