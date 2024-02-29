NAME		=	scop
SRC			=	main.cpp\
				KeyboardController.cpp\
				MainClass.cpp\
				SimpleRenderSystem.cpp\
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
GLSLC		=	./glslc

# Flags
CPPFLAGS	=	-std=c++17 -g3 -MMD
LDFLAGS		=	-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Path
INC			=	-Iglm -Iinc -Iinc/VulkanEngine
SRC_PATH	=	./src/
OBJ_PATH	=	./obj/
OBJ_PATH_VE	=	./obj/VulkanEngine

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

${OBJ_PATH}%.o:${SRC_PATH}%.cpp
	${CPP} ${CPPFLAGS} ${INC} -c $< -o $@

${OBJ_DIRS}:
	mkdir ${OBJ_DIRS} ${OBJ_PATH_VE}

${NAME}:${OBJ_DIRS} ${OBJ} shaderclean
	${CPP} ${OBJ} ${LDFLAGS} -o $@
	${GLSLC} ./shader/simpleShader.vert -o ./shader/simpleShader.vert.spv
	${GLSLC} ./shader/simpleShader.frag -o ./shader/simpleShader.frag.spv

.PHONY:all clean shaderclean fclean re run

-include ${DEP}
