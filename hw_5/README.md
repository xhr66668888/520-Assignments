# HW5 - Supervised and Unsupervised ML with C++

This folder has two programs:
- `kmeans.cpp` (unsupervised: K-Means clustering)
- `regression.cpp` (supervised: simple linear regression)

## Compile
Use g++ (C++17):

```bash
g++ -std=c++17 -o kmeans kmeans.cpp
g++ -std=c++17 -o regression regression.cpp
```

## Run
```bash
./kmeans
./regression
```

## Output files
- K-Means image: `output.ppm`
- Regression image: `regression.ppm`

Both programs generate synthetic 2D data in `main()`, run the full algorithm, print basic metrics/logs, and save the `.ppm` visualization file.

## Notes
- Synthetic data generation follows the sample logic provided in the HW5 prompt (cluster-center + Gaussian noise for K-Means, and line + noise for regression).
- PPM output is written manually using standard file I/O so no external graphics library is needed.
