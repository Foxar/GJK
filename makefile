#OBJS specifies which files to compile as part of the project
OBJS = main.cpp glad.c

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = opengl

#This is the target that compiles our executable
all : $(OBJS)
	g++ $(OBJS) -IC:\mingw_dev_lib\glfw\include -IC:\mingw_dev_lib\glm\glm -IC:\mingw_dev_lib\freeglut\include -IC:\mingw_dev_lib\glad -LC:\mingw_dev_lib\freeglut\lib -LC:\mingw_dev_lib\glfw\lib-mingw -LC:\mingw_dev_lib\glew\lib\Release\Win32 -w -Wl,-subsystem,console -lglfw3dll -lglfw3 -lOpenGL32 -lglu32 -lfreeGLUT -lglew32 -o $(OBJ_NAME)
