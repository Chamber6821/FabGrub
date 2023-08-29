//
// Created by Ivan on 05.08.2023.
//

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
#include "package/MemPackage.h"
#include "re146/FileRepository.h"
#include "re146/Repository.h"
#include "repository/MemRepository.h"
#include "repository/OverloadedRepository.h"
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
        auto log = make<ForkedLog>(
            make<StreamLog>(std::shared_ptr<std::ostream>(
                std::shared_ptr<std::nullptr_t>(),
                &std::cout
            )),
            make<StreamLog>(make<std::ofstream>("log.txt"))
        );
        auto http = make<LoggedHttp>(make<HttpClient>(), log);
        auto basePackage = make<MemPackage>("base", "1.1.80");

        // fixes text of std::filesystem::filesystem_error
        // NOLINTNEXTLINE(*-mt-unsafe)
        (void)std::setlocale(LC_ALL, "");
        // NOLINTNEXTLINE(*-mt-unsafe)
        log->info("Current locale: {}", std::setlocale(LC_ALL, nullptr));

        try {
            make<SequencialFilling>(
                make<LoggedDestination>(
                    make<DestinationDirectory>(
                        "mods",
                        make<OverloadedFileRepository>(
                            basePackage,
                            make<FakeFile>(),
                            make<FileCachedFileRepository>(
                                "mods-cached",
                                make<re146::FileRepository>(http)
                            )
                        )
                    ),
                    log
                ),
                make<PubgrubSolution>(
                    make<MemRequirements>(make<MemRequirement>(
                        "space-exploration",
                        "0.0.0",
                        "1.0.0"
                    )),
                    make<OverloadedRepository>(
                        make<re146::Repository>(make<MemCachedHttp>(http)),
                        "base",
                        make<MemPackages>(basePackage)
                    )
                )
            )
                ->fill();
        } catch (const std::exception &e) {
            log->info("{}", stringify(e, 0));
            throw e;
        }
    }
}
