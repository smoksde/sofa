g++ -o simulator simulator.cpp fourier.cpp -I/home/smoks/anaconda3/include/python3.9 -I/home/smoks/anaconda3/lib/python3.9/site-packages/numpy/core/include -L/home/smoks/anaconda3/lib -lpython3.9 -lSDL2 -lGL -lGLEW
export LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:/home/smoks/anaconda3/lib:$LD_LIBRARY_PATH
./simulator
