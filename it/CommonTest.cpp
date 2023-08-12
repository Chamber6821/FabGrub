//
// Created by Ivan on 05.08.2023.
//

#include "destination/MemDestination.h"
#include "doctest-nolint.h"
#include "package/MemPackage.h"
#include "repository/MemRepository.h"
#include "requirement/MemRequirement.h"
#include "solution/PubgrubSolution.h"

TEST_SUITE("Common integration test") {
    TEST_CASE("") {
        auto destination = make<MemDestination>();

        destination->fill(
            make<PubgrubSolution>(
                make<MemRequirements>(
                    make<MemRequirement>("a", "1.0.0", "2.0.0")
                ),
                make<MemRepository>(std::pair{
                    "a",
                    make<MemPackages>(
                        make<MemPackage>("a", "1.0.0", "bytes-for-1.0.0"),
                        make<MemPackage>("a", "1.0.1", "bytes-for-1.0.1"),
                        make<MemPackage>("a", "1.0.2", "bytes-for-1.0.2"),
                        make<MemPackage>("a", "1.1.0", "bytes-for-1.1.0"),
                        make<MemPackage>("a", "2.0.0", "bytes-for-2.0.0")
                    )})
            )
                ->packages()
        );

        CHECK(destination->count() == 1);
        auto package = destination->at(0);
        CHECK(package->name() == "a");
        CHECK(*package->version() == VersionOf("1.1.0"));
    }
}
