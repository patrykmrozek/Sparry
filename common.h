#include <stdint.h>

#define PI 3.14159265359f


#define NEAR 1.0f
#define FAR 100000.0f
#define FOV DEG_TO_RAD(90) 

#define DEG_TO_RAD(_d) ((_d) * (PI / 180)) 
#define RAD_TO_DEG(_d) ((_d) * (180 / PI))

#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600

typedef uint32_t u32;
typedef int i32;
typedef float f32;


