g++ -o dft dft_example.cpp fourier.cpp -I/home/smoks/anaconda3/include/python3.9 -I/home/smoks/anaconda3/lib/python3.9/site-packages/numpy/core/include -L/home/smoks/anaconda3/lib -lpython3.9
export LD_LIBRARY_PATH=/home/smoks/anaconda3/lib:$LD_LIBRARY_PATH
./dft