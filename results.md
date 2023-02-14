```
                                                  |              INTEL              |                M1               |
    CURVE   | PubKey Lgth | Total B/clu | Total B | Detection time  | Detection/msg | Detection time  | Detection/msg |
 secp224r1  |  29 bytes   |  45 bytes   | 22.5 MB | 30'571 ± 31  ms |   0.0611 ms   | 24'381 ± 71  ms |   0.0487 ms   |
 sect113r1  |  16 bytes   |  32 bytes   | 16.0 MB | 42'921 ± 125 ms |   0.0858 ms   | 33'564 ± 126 ms |   0.0671 ms   |
 sect113r2  |  16 bytes   |  32 bytes   | 16.0 MB | 42'931 ± 163 ms |   0.0858 ms   | 33'607 ± 102 ms |   0.0672 ms   |
 prime256v1 |  33 bytes   |  49 bytes   | 24.5 MB | 25'705 ± 17  ms |   0.0514 ms   | 20'125 ± 55  ms |   0.0402 ms   |
```

## Intel CPU (3rd Gen Intel Xeon Ice Lake with AVX-512) – M6i AWS Instance

```plaintext
Average keygen: 33.107707 ms ± 0.297530 ms
Average msggen: 30681.407267 ms ± 13.501815 ms
Average detect: 30570.918287 ms ± 31.020218 ms (0.061142 per msg)
Average fpcount: 7.820000 ± 3.521496

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r1 curve   150 iterations
Public key length: 16 bytes
Average keygen: 92.102133 ms ± 0.381658 ms
Average msggen: 43012.493667 ms ± 88.977694 ms
Average detect: 42921.146580 ms ± 125.190539 ms (0.085842 per msg)
Average fpcount: 7.533333 ± 3.395422

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r2 curve   150 iterations
Public key length: 16 bytes
Average keygen: 92.064273 ms ± 0.147657 ms
Average msggen: 42997.829127 ms ± 89.897008 ms
Average detect: 42931.644267 ms ± 163.628695 ms (0.085863 per msg)
Average fpcount: 7.526667 ± 3.726834

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_X9_62_prime256v1 curve   150 iterations
Public key length: 33 bytes
^[[B^[[AAverage keygen: 19.139227 ms ± 0.052493 ms
Average msggen: 25835.174453 ms ± 19.753817 ms
Average detect: 25705.980907 ms ± 17.585414 ms (0.051412 per msg)
Average fpcount: 7.326667 ± 3.215580
```

## M1 Max ARM64 CPU – MacBook Pro

```plaintext
BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_secp224r1 curve   150 iterations
Public key length: 29 bytes
Average keygen: 25.125073 ms ± 0.200887 ms
Average msggen: 24447.530353 ms ± 73.433398 ms
Average detect: 24381.200707 ms ± 71.998493 ms (0.048762 per msg)
Average fpcount: 7.773333 ± 3.777383

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r1 curve   150 iterations
Public key length: 16 bytes
Average keygen: 70.755393 ms ± 0.391482 ms
Average msggen: 33629.876960 ms ± 104.124172 ms
Average detect: 33564.608873 ms ± 126.541040 ms (0.067129 per msg)
Average fpcount: 7.226667 ± 3.419838

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r2 curve   150 iterations
Public key length: 16 bytes
Average keygen: 70.727073 ms ± 0.307893 ms
Average msggen: 33690.228527 ms ± 101.208111 ms
Average detect: 33607.865313 ms ± 102.292264 ms (0.067216 per msg)
Average fpcount: 7.426667 ± 3.293117

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_X9_62_prime256v1 curve   150 iterations
Public key length: 33 bytes
Average keygen: 14.046173 ms ± 0.077297 ms
Average msggen: 20182.867500 ms ± 56.742144 ms
Average detect: 20125.602473 ms ± 55.736033 ms (0.040251 per msg)
Average fpcount: 7.453333 ± 2.996858
```