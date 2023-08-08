//
// Created by Ivan on 05.08.2023.
//

#include "App.h"
#include "destination/MemDestination.h"
#include "doctest-nolint.h"
#include "package/MemPackage.h"
#include "package/MemPackages.h"
#include "profile/MemProfile.h"
#include "profile/MemProfiles.h"
#include "repository/MemRepository.h"
#include "requirement/MemRequirement.h"
#include "requirement/MemRequirements.h"

TEST_SUITE("Common integration test") {
    TEST_CASE("") {
        auto destination = make<MemDestination>();

        make<App>(
            make<MemProfiles>(make<MemProfile>(
                "profile",
                make<MemRequirements>(
                    make<MemRequirement>("a", "1.0.0", "2.0.0")
                )
            )),
            make<MemRepository>(std::pair{
                "a",
                make<MemPackages>(
                    make<MemPackage>("a", "1.0.0", "bytes-for-1.0.0"),
                    make<MemPackage>("a", "1.0.1", "bytes-for-1.0.1"),
                    make<MemPackage>("a", "1.0.2", "bytes-for-1.0.2"),
                    make<MemPackage>("a", "1.1.0", "bytes-for-1.1.0"),
                    make<MemPackage>("a", "2.0.0", "bytes-for-2.0.0")
                )}),
            destination
        )
            ->load("profile");

        CHECK(destination->count() == 1);
        auto package = destination->at(0);
        CHECK(package->name() == "a");
        CHECK(*package->version() == VersionOf("1.1.0"));
    }
}
