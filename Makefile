OUT = sparry
#add -DCELP_DEBUG to enable logging
$(OUT):
	gcc \
		src/main.c \
		src/raster.c \
		src/render.c \
		src/camera.c \
		src/transform.c \
		src/input.c \
		\
		-Iinclude\
		-lSDL2 -lm \
		-Wall -Wextra \
		-o $(OUT) 

run: $(OUT)
	./$(OUT)
