g++ -o optimizer optimizer.cpp simulator.cpp -lpython3.10 -lSDL2 -lGL -lGLEW
export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:/home/smoks/anaconda3/lib:$LD_LIBRARY_PATH
./optimizer
