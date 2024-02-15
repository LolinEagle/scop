NAME		=	scop
SRC			=	main.cpp\
				MainClass.cpp\
				SimpleRenderSystem.cpp\
				VeDevice.cpp\
				VeGameObject.cpp\
				VeModel.cpp\
				VePipeline.cpp\
				VeRenderer.cpp\
				VeSwapChain.cpp\
				VeWindow.cpp

# Commands
CPP			=	c++
RM			=	rm -rf
GLSLC		=	./shader/glslc

# Flags
CPPFLAGS	=	-std=c++17 -g3 -MMD
LDFLAGS		=	-lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi

# Path
INC			=	-Iglm -Iinc
SRC_PATH	=	./src/
OBJ_PATH	=	./obj/

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
	mkdir ${OBJ_DIRS}

${NAME}:${OBJ_DIRS} ${OBJ} shaderclean
	${CPP} ${OBJ} ${LDFLAGS} -o $@
	${GLSLC} ./shader/simpleShader.vert -o ./shader/simpleShader.vert.spv
	${GLSLC} ./shader/simpleShader.frag -o ./shader/simpleShader.frag.spv

.PHONY:all clean shaderclean fclean re run

-include ${DEP}
