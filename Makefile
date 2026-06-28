OUT = sparry
$(OUT):
	gcc \
		src/main.c \
		src/raster.c \
		src/render.c \
		src/camera.c \
		src/transform.c \
		src/input.c \
		\
		-lSDL2 -lm \
		-Wall -Wextra \
		-o $(OUT) 

run: $(OUT)
	./$(OUT)
