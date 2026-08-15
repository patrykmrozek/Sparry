OUT = sparry

$(OUT):
	gcc \
		-DLOG_MODE_ERROR \
		src/main.c \
		src/aabb.c \
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
