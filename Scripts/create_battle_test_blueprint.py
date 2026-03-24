import unreal


ASSET_PATH = "/Game/BP_BattleTestActor"
ASSET_NAME = "BP_BattleTestActor"
PACKAGE_PATH = "/Game"


def main():
    existing = unreal.EditorAssetLibrary.load_asset(ASSET_PATH)
    if existing:
        unreal.log(f"Asset already exists: {ASSET_PATH}")
        unreal.EditorAssetLibrary.save_loaded_asset(existing)
        return

    factory = unreal.BlueprintFactory()
    factory.set_editor_property("ParentClass", unreal.BattleTestActor)

    asset_tools = unreal.AssetToolsHelpers.get_asset_tools()
    bp = asset_tools.create_asset(ASSET_NAME, PACKAGE_PATH, unreal.Blueprint, factory)

    if not bp:
        raise RuntimeError("Failed to create BP_BattleTestActor")

    unreal.EditorAssetLibrary.save_loaded_asset(bp)
    unreal.log(f"Created asset: {ASSET_PATH}")


if __name__ == "__main__":
    main()
