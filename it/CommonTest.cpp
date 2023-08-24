//
// Created by Ivan on 05.08.2023.
//

#include "destination/DestinationDirectory.h"
#include "destination/LoggedDestination.h"
#include "destination/MemDestination.h"
#include "doctest-nolint.h"
#include "file/FakeFile.h"
#include "http/HttpClient.h"
#include "http/LoggedHttp.h"
#include "http/MemCachedHttp.h"
#include "log/ForkedLog.h"
#include "log/StreamLog.h"
#include "package/MemPackage.h"
#include "repository/MemRepository.h"
#include "repository/OverloadedRepository.h"
#include "repository/re146/Repository.h"
#include "requirement/MemRequirement.h"
#include "solution/PubgrubSolution.h"

auto operator<<(std::ostream &out, const Package &p) -> std::ostream & {
    return out << fmt::format("{}:{}", p.name(), fmt::streamed(*p.version()));
}

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
        auto output = std::shared_ptr<std::ostream>(
            std::shared_ptr<std::nullptr_t>(),
            &std::cout
        );
        auto log = make<ForkedLog>(
            make<StreamLog>(output),
            make<StreamLog>(make<std::ofstream>("log.txt"))
        );

        try {
            make<LoggedDestination>(make<DestinationDirectory>("mods"), log)
                ->fill(make<PubgrubSolution>(
                           make<MemRequirements>(make<MemRequirement>(
                               "space-exploration",
                               "0.0.0",
                               "1.0.0"
                           )),
                           make<OverloadedRepository>(
                               make<re146::Repository>(make<MemCachedHttp>(
                                   make<LoggedHttp>(make<HttpClient>(), log)
                               )),
                               "base",
                               make<MemPackages>(make<MemPackage>(
                                   "base",
                                   "1.1.80",
                                   make<FakeFile>()
                               ))
                           )
                )
                           ->packages());
        } catch (const std::exception &e) {
            std::cout << stringify(e, 0) << '\n';
            throw e;
        }
    }
}
