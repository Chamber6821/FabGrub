include(FetchContent)

FetchContent_Declare(
        doctest
        GIT_REPOSITORY https://github.com/doctest/doctest.git
        GIT_TAG v2.4.9
)

FetchContent_Declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.0.0
)

FetchContent_Declare(
        Reactions
        GIT_REPOSITORY https://github.com/Chamber6821/Reactions.git
        GIT_TAG 0.1.0
)

FetchContent_Declare(
        pubgrub
        GIT_REPOSITORY https://github.com/Chamber6821/pubgrub-cmake.git
        GIT_TAG 0.3.2-cmake
)

FetchContent_Declare(
        cpr
        GIT_REPOSITORY https://github.com/libcpr/cpr.git
        GIT_TAG d7cec83e9480253bacf11ff6441e9d00c08c71bf # Fix for branch "1.10.x"
)
