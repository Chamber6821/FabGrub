//
// Created by Ivan on 05.08.2023.
//

#include "doctest-nolint.h"

TEST_SUITE("Common integration test") {
    TEST_CASE("") {
        auto destination = make<MemDestination>();

        make<App>(
            make<MemProfiles>(make<MemProfile>(
                "profile",
                make<Requirements>(
                    make<Requirement>("a", make<Interval>("1.0.0", "2.0.0"))
                )
            )),
            make<MemRepository>(
                make<MemPackage>("a", "1.0.0", {}, "bytes-for-1.0.0"),
                make<MemPackage>("a", "1.0.1", {}, "bytes-for-1.0.1"),
                make<MemPackage>("a", "1.0.2", {}, "bytes-for-1.0.2"),
                make<MemPackage>("a", "1.1.0", {}, "bytes-for-1.1.0"),
                make<MemPackage>("a", "2.0.0", {}, "bytes-for-2.0.0")
            ),
            destination
        )
            ->load("profile");

        CHECK(destination->count() == 1);
        CHECK(destination->containsWithVersion("a", "1.1.0"));
}
}
