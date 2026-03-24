import unreal


MAP_PATH = "/Game/Maps/BattleTestMap"
BP_PATH = "/Game/BP_BattleTestActor"
ACTOR_LABEL = "BP_BattleTestActor_Auto"


def ensure_map_loaded():
    if unreal.EditorAssetLibrary.does_asset_exist(MAP_PATH):
        if not unreal.EditorLevelLibrary.load_level(MAP_PATH):
            raise RuntimeError(f"Failed to load level: {MAP_PATH}")
        unreal.log(f"Loaded existing level: {MAP_PATH}")
    else:
        if not unreal.EditorLevelLibrary.new_level(MAP_PATH):
            raise RuntimeError(f"Failed to create level: {MAP_PATH}")
        unreal.log(f"Created new level: {MAP_PATH}")


def ensure_test_actor():
    bp_asset = unreal.EditorAssetLibrary.load_asset(BP_PATH)
    if not bp_asset:
        raise RuntimeError(f"Battle test blueprint not found: {BP_PATH}")

    bp_class = bp_asset.generated_class()
    if not bp_class:
        raise RuntimeError(f"Could not resolve generated class for: {BP_PATH}")

    for actor in unreal.EditorLevelLibrary.get_all_level_actors():
        if actor.get_actor_label() == ACTOR_LABEL:
            unreal.log(f"Actor already exists: {ACTOR_LABEL}")
            return

    actor = unreal.EditorLevelLibrary.spawn_actor_from_class(
        bp_class, unreal.Vector(0.0, 0.0, 120.0), unreal.Rotator(0.0, 0.0, 0.0)
    )
    actor.set_actor_label(ACTOR_LABEL)
    unreal.log(f"Spawned actor: {ACTOR_LABEL}")


def save_level_and_set_defaults():
    if not unreal.EditorLevelLibrary.save_current_level():
        raise RuntimeError("Failed to save current level")
    unreal.log("Saved level. Default maps are configured in Config/DefaultEngine.ini.")


def main():
    ensure_map_loaded()
    ensure_test_actor()
    save_level_and_set_defaults()


if __name__ == "__main__":
    main()
