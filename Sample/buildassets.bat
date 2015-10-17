"../OakNut/tools/bin/FBXConverter.exe" srcAssets/meshes/cube.fbx assets/meshes/cube.mesh -color on
"../OakNut/tools/bin/FBXConverter.exe" srcAssets/meshes/sphere.fbx assets/meshes/sphere.mesh
"../OakNut/tools/bin/FBXConverter.exe" srcAssets/meshes/ogre.fbx assets/meshes/ogre.mesh -up -y -bump on
"../OakNut/tools/bin/FBXConverter.exe" srcAssets/meshes/floor.fbx assets/meshes/floor.mesh -bump on

"../OakNut/tools/bin/Normalizer.exe" srcAssets/textures/Dungeon1Floor_height.png assets/textures/Dungeon1Floor_norm.png
PAUSE
