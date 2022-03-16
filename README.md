This is terrain generation tool written with OpenGL. It is based on Perlin Noise. Users can interract with program and generate numbers of worlds using widgets.

Project has dependencies on specific libraries which are GLFW, GLM, Glad, Imgui and siv::PerlinNoise. Unfortunately this dependencies resolved by package manager called vcpkg so project does not contain necessary headers, source and binaries. An individuals that wants to run the project, must resolve dependencies by themselves.

Project exclude advanced graphics techniques to in sake of learning 3D physics. Although project contains some OpenGL code, anyone with no prior 3D Programming knowledge can understand Physics concepts.