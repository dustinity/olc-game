import unreal


MAP_PACKAGE = "/Game/Maps/MainMenu"


def ensure_actor(actor_class, label, location=None, rotation=None):
    actors = unreal.EditorLevelLibrary.get_all_level_actors()
    for actor in actors:
        if actor.get_actor_label() == label:
            return actor

    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        actor_class,
        location or unreal.Vector(0.0, 0.0, 0.0),
        rotation or unreal.Rotator(0.0, 0.0, 0.0),
    )
    actor.set_actor_label(label)
    return actor


def main():
    unreal.EditorLevelLibrary.new_level(MAP_PACKAGE)

    world_settings = unreal.EditorLevelLibrary.get_editor_world().get_world_settings()
    menu_game_mode = unreal.load_class(None, "/Script/OurLastChance.OLCMenuGameMode")
    if menu_game_mode:
        world_settings.set_editor_property("default_game_mode", menu_game_mode)

    ensure_actor(
        unreal.DirectionalLight,
        "MainMenu_DirectionalLight",
        unreal.Vector(0.0, 0.0, 600.0),
        unreal.Rotator(-45.0, 35.0, 0.0),
    )
    ensure_actor(
        unreal.SkyLight,
        "MainMenu_SkyLight",
        unreal.Vector(0.0, 0.0, 400.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )
    ensure_actor(
        unreal.PlayerStart,
        "MainMenu_PlayerStart",
        unreal.Vector(0.0, 0.0, 100.0),
        unreal.Rotator(0.0, 0.0, 0.0),
    )

    gameplay_statics = unreal.GameplayStatics
    unreal.EditorLoadingAndSavingUtils.save_dirty_packages(True, True)
    unreal.EditorAssetLibrary.save_asset(MAP_PACKAGE, only_if_is_dirty=False)
    unreal.log("[OLC] Created and saved /Game/Maps/MainMenu")


if __name__ == "__main__":
    main()
