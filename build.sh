files=$(find src | grep .cpp)
g++ -lglfw -framework OpenGL -lglew -DGLEW_STATIC --std=c++11 -O3 $files -o build.out