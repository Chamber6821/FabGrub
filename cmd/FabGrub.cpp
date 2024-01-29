#include "action/Executable.h"
#include "action/ProtectedPath.h"
#include "action/Sequence.h"
#include "action/SequentialFilling.h"
#include "cancellation/CtrlCCancellation.h"
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
#include "package/PackageFromModInfo.h"
#include "profile/JsonProfile.h"
#include "re146/FileRepository.h"
#include "re146/Repository.h"
#include "repository/MemRepository.h"
#include "repository/OverloadedRepository.h"
#include "scalar/JsonFromFile.h"
#include "solution/FileCachedSolution.h"
#include "solution/PubgrubSolution.h"
#include "utils/locale.h"
#include "utils/stringify.h"
#include <span>

auto main(int argc, char **argv) -> int {
    ptr<Log> log = make<StreamLog>(std::cout);
    try {
        auto args = std::span(argv, static_cast<size_t>(argc));
        if (args.size() != 2)
            throw std::runtime_error(fmt::format(
                "FabGrub expect one console argument, but got {}",
                args.size() - 1
            ));
        auto profileName = std::string_view(args[1]);

        auto root = std::filesystem::path(".");
        auto home = root / "fabgrub";
        auto profiles = home / "profiles";
        auto logFile = home / "log.txt";
        auto profileFile = profiles / fmt::format("{}.json", profileName);
        auto profileLockFile =
            profiles / fmt::format("{}.lock.json", profileName);
        auto basePackageFile = root / "data" / "base" / "info.json";
        auto mods = root / "mods";
        auto settings = mods / "mod-settings.dat";
        auto protectedUserMods = home / "mods-original";
        auto protectedUserSettings =
            profiles / fmt::format("{}.settings.dat", profileName);
        auto cachedMods = home / "mods";
        auto factorioExecutable = root / "bin" / "x64" / "factorio";

        log =
            make<SynchronizedLog>(make<ForkedLog>(log, make<FileLog>(logFile)));

        set_locale("");
        log->info("Current locale: {}", get_locale());

        auto profile = make<JsonProfile>(make<JsonFromFile>(profileFile));
        log->info("Chosen profile: {}", profileName);

        auto basePackage = make<PackageFromModInfo>(basePackageFile);

        log->info(
            "Detected Factorio version: {}",
            fmt::streamed(*basePackage->version())
        );

        auto http = make<LoggedHttp>(log, make<HttpClient>());
        auto app = make<ProtectedPath>(
            mods,
            protectedUserMods,
            log,
            make<ProtectedPath>(
                protectedUserSettings,
                settings,
                log,
                make<Sequence>(
                    make<SequentialFilling>(
                        make<CtrlCCancellation>(),
                        make<LoggedDestination>(
                            log,
                            make<DestinationDirectory>(
                                mods,
                                make<OverloadedFileRepository>(
                                    basePackage,
                                    make<FakeFile>(),
                                    make<FileCachedFileRepository>(
                                        cachedMods,
                                        make<re146::FileRepository>(http)
                                    )
                                )
                            )
                        ),
                        make<FileCachedSolution>(
                            std::filesystem::last_write_time(profileFile),
                            profileLockFile,
                            make<PubgrubSolution>(
                                make<CtrlCCancellation>(),
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
                    make<Executable>(factorioExecutable)
                )
            )
        );

        (*app)();
    } catch (const std::exception &e) {
        try {
            try {
                log->info("{}", stringify(e, 0));
                return 1;
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
}
