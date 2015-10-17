"../OakNut/tools/bin/FBXConverter.exe" srcAssets/meshes/cube.fbx assets/meshes/cube.mesh -color on
"../OakNut/tools/bin/FBXConverter.exe" srcAssets/meshes/sphere.fbx assets/meshes/sphere.mesh
"../OakNut/tools/bin/FBXConverter.exe" srcAssets/meshes/ogre.fbx assets/meshes/ogre.mesh -up -y -bump on
"../OakNut/tools/bin/FBXConverter.exe" srcAssets/meshes/floor.fbx assets/meshes/floor.mesh -bump on
"../OakNut/tools/bin/FBXConverter.exe" srcAssets/meshes/floor2.fbx assets/meshes/floor2.mesh -bump on
"../OakNut/tools/bin/FBXConverter.exe" srcAssets/meshes/wall1.fbx assets/meshes/wall1.mesh -bump on

"../OakNut/tools/bin/Normalizer.exe" srcAssets/textures/Dungeon1Floor_height.png assets/textures/Dungeon1Floor_norm.png
"../OakNut/tools/bin/Normalizer.exe" srcAssets/textures/Dungeon2Floor_height.png assets/textures/Dungeon2Floor_norm.png
"../OakNut/tools/bin/Normalizer.exe" srcAssets/textures/Wall1_height.png assets/textures/Wall1_norm.png
PAUSE
