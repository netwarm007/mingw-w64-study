add_executable(helloworld_opengl WIN32
graphicsclass.cpp 
inputclass.cpp 
main.cpp 
openglclass.cpp 
systemclass.cpp 
colorshaderclass.cpp 
modelclass.cpp 
cameraclass.cpp
)
target_link_libraries(helloworld_opengl
	Opengl32
)
