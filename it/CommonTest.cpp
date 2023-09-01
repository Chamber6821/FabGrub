//
// Created by Ivan on 05.08.2023.
//

#include "action/SequentialFilling.h"
#include "destination/DestinationDirectory.h"
#include "destination/LoggedDestination.h"
#include "doctest-nolint.h"
#include "file-repository/FileCachedFileRepository.h"
#include "file-repository/OverloadedFileRepository.h"
#include "file/FakeFile.h"
#include "http/HttpClient.h"
#include "http/LoggedHttp.h"
#include "http/MemCachedHttp.h"
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

TEST_SUITE("Common integration test") {
    TEST_CASE("") {
        auto log = make<SynchronizedLog>(make<ForkedLog>(
            make<StreamLog>(std::shared_ptr<std::ostream>(
                std::shared_ptr<std::nullptr_t>(),
                &std::cout
            )),
            make<StreamLog>(make<std::ofstream>("log.txt"))
        ));

        try {
            auto profileName = std::string_view("test");
            auto profileFile = std::filesystem::path(
                fmt::format("profiles/{}.json", profileName)
            );
            auto profile = make<JsonProfile>(make<JsonFromFile>(profileFile));

            // fixes text of std::filesystem::filesystem_error
            // NOLINTNEXTLINE(*-mt-unsafe)
            (void)std::setlocale(LC_ALL, "");
            // NOLINTNEXTLINE(*-mt-unsafe)
            log->info("Current locale: {}", std::setlocale(LC_ALL, nullptr));
            log->info("Chosen profile: {}", profileName);
            log->info(
                "Chosen Factorio version: {}",
                fmt::streamed(*profile->factorioVersion())
            );

            auto basePackage =
                make<MemPackage>("base", profile->factorioVersion());
            auto http = make<LoggedHttp>(log, make<HttpClient>());
            auto app = make<SequentialFilling>(
                make<LoggedDestination>(
                    log,
                    make<DestinationDirectory>(
                        "mods",
                        make<OverloadedFileRepository>(
                            basePackage,
                            make<FakeFile>(),
                            make<FileCachedFileRepository>(
                                "mods-cached-by-fabgrub",
                                make<re146::FileRepository>(http)
                            )
                        )
                    )
                ),
                make<FileCachedSolution>(
                    std::filesystem::last_write_time(profileFile),
                    fmt::format("profiles/{}.lock.json", profileName),
                    make<PubgrubSolution>(
                        profile->requirements(),
                        make<OverloadedRepository>(
                            basePackage->name(),
                            make<MemPackages>(basePackage),
                            make<re146::Repository>(make<MemCachedHttp>(http))
                        )
                    )
                )
            );

            (*app)();
        } catch (const std::exception &e) {
            log->info("{}", stringify(e, 0));
            throw e;
        }
    }
}
