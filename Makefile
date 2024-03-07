NAME		=	scop
SRC			=	main.cpp\
				Controller.cpp\
				MainClass.cpp\
				PointLight.cpp\
				SimpleRender.cpp\
				VulkanEngine/VeBuffer.cpp\
				VulkanEngine/VeCamera.cpp\
				VulkanEngine/VeDescriptors.cpp\
				VulkanEngine/VeDevice.cpp\
				VulkanEngine/VeGameObject.cpp\
				VulkanEngine/VeModel.cpp\
				VulkanEngine/VePipeline.cpp\
				VulkanEngine/VeRenderer.cpp\
				VulkanEngine/VeSwapChain.cpp\
				VulkanEngine/VeWindow.cpp

# Commands
CPP			=	c++
RM			=	rm -rf
GLSLC		=	./shader/glslc

# Flags
CPPFLAGS	=	-std=c++17 -MMD
LDFLAGS		=	-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Path
INC			=	-Iglm -Iinc -Iinc/VulkanEngine
SRC_PATH	=	./src/
OBJ_PATH	=	./obj/
OBJ_PATH_VE	=	./obj/VulkanEngine/

# Objects
OBJ_DIRS	=	${OBJ_PATH}
OBJ			=	${addprefix ${OBJ_PATH},${SRC:.cpp=.o}}
DEP			=	${addprefix ${OBJ_PATH},${SRC:.cpp=.d}}

all:${NAME}

clean:
	${RM} ${OBJ_PATH} .vscode

shaderclean:
	${RM} ./shader/*.spv

fclean:clean shaderclean
	${RM} ${NAME} 

re:fclean
	make all

run:${NAME}
	clear
	./${NAME}

shader:shaderclean
	${GLSLC} ./shader/simpleShader.vert -o ./shader/simpleShader.vert.spv
	${GLSLC} ./shader/simpleShader.frag -o ./shader/simpleShader.frag.spv
	${GLSLC} ./shader/pointLight.vert -o ./shader/pointLight.vert.spv
	${GLSLC} ./shader/pointLight.frag -o ./shader/pointLight.frag.spv

${OBJ_PATH}%.o:${SRC_PATH}%.cpp
	${CPP} ${CPPFLAGS} ${INC} -c $< -o $@

${OBJ_DIRS}:
	mkdir ${OBJ_DIRS} ${OBJ_PATH_VE}

${NAME}:${OBJ_DIRS} ${OBJ} shader
	${CPP} ${OBJ} ${LDFLAGS} -o $@

.PHONY:all clean shaderclean fclean re run shader

-include ${DEP}
