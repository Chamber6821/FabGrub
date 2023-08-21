//
// Created by Ivan on 05.08.2023.
//

#include "destination/MemDestination.h"
#include "doctest-nolint.h"
#include "http/HttpClient.h"
#include "http/LoggedHttp.h"
#include "log/ForkedLog.h"
#include "log/StreamLog.h"
#include "package/MemPackage.h"
#include "repository/MemRepository.h"
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
        auto destination = make<MemDestination>();
        auto log = make<ForkedLog>(
            make<StreamLog>(std::shared_ptr<std::ostream>(
                std::shared_ptr<nullptr_t>(),
                &std::cout
            )),
            make<StreamLog>(make<std::ofstream>("log.txt"))
        );

        try {
            destination->fill(
                make<PubgrubSolution>(
                    make<MemRequirements>(make<MemRequirement>(
                        "space-exploration",
                        "1.0.0",
                        "2.0.0"
                    )),
                    make<MemCachedRepository>(make<Re146Repository>(
                        make<LoggedHttp>(make<HttpClient>(), log)
                    ))
                )
                    ->packages()
            );

            for (const auto &p : to_range<ptr<Package>>(destination)) {
                std::cout << *p << '\n';
            }
        } catch (const std::exception &e) {
            std::cout << stringify(e, 0) << '\n';
            throw e;
        }
    }
}
