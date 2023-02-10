```
                                                  |              INTEL              |                M1               |
    CURVE   | PubKey Lgth | Total B/clu | Total B | Detection time  | Detection/msg | Detection time  | Detection/msg |
 secp224r1  |  29 bytes   |  45 bytes   | 22.5 MB | 30'562 ± 24  ms |   0.0611 ms   | 25'064 ± 316 ms |   0.0501 ms   |
 sect113r1  |  16 bytes   |  32 bytes   | 16.0 MB | 43'011 ± 287 ms |   0.086  ms   | 34'639 ± 436 ms |   0.0692 ms   |
 sect113r2  |  16 bytes   |  32 bytes   | 16.0 MB | 43'051 ± 312 ms |   0.0861 ms   | 34'579 ± 422 ms |   0.0691 ms   |
 prime256v1 |  33 bytes   |  49 bytes   | 24.5 MB | 25'708 ± 17  ms |   0.0514 ms   | 20'715 ± 351 ms |   0.0414 ms   |
```

## Intel CPU (3rd Gen Intel Xeon Ice Lake with AVX-512) – M6i AWS Instance

```plaintext
BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_secp224r1 curve   150 iterations
Public key length: 29 bytes
Average keygen: 33.112187 ms ± 0.310584 ms
Average msggen: 30707.692807 ms ± 30.267664 ms
Average detect: 30562.543233 ms ± 23.679402 ms
Average fpcount: 6.953333 ± 3.099111

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r1 curve   150 iterations
Public key length: 16 bytes
Average keygen: 92.196807 ms ± 0.343168 ms
Average msggen: 43184.507673 ms ± 366.205772 ms
Average detect: 43011.505747 ms ± 286.878050 ms
Average fpcount: 7.666667 ± 3.265306

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r2 curve   150 iterations
Public key length: 16 bytes
Average keygen: 92.108660 ms ± 0.235890 ms
Average msggen: 43214.919260 ms ± 336.271473 ms
Average detect: 43050.976540 ms ± 312.483930 ms
Average fpcount: 7.540000 ± 3.172024

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_X9_62_prime256v1 curve   150 iterations
Public key length: 33 bytes
Average keygen: 19.112727 ms ± 0.063738 ms
Average msggen: 25817.312753 ms ± 23.923591 ms
Average detect: 25708.575387 ms ± 17.522683 ms
Average fpcount: 7.640000 ± 3.290957
```

## M1 Max Macbook Pro ARM64 CPU

```plaintext
BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_secp224r1 curve   150 iterations
Public key length: 29 bytes
Average keygen: 27.897473 ms ± 0.914330 ms
Average msggen: 25121.695340 ms ± 326.592272 ms
Average detect: 25064.147513 ms ± 315.504736 ms
Average fpcount: 7.446667 ± 3.387303

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r1 curve   150 iterations
Public key length: 16 bytes
Average keygen: 74.563787 ms ± 0.923962 ms
Average msggen: 34795.896347 ms ± 486.864327 ms
Average detect: 34639.369987 ms ± 435.774616 ms
Average fpcount: 8.146667 ± 3.663126

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r2 curve   150 iterations
Public key length: 16 bytes
Average keygen: 74.354833 ms ± 0.889370 ms
Average msggen: 34724.430200 ms ± 439.475611 ms
Average detect: 34579.443780 ms ± 422.757861 ms
Average fpcount: 7.713333 ± 3.088336

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_X9_62_prime256v1 curve   150 iterations
Public key length: 33 bytes
Average keygen: 16.443920 ms ± 0.301233 ms
Average msggen: 20804.817187 ms ± 425.985744 ms
Average detect: 20715.277453 ms ± 351.040008 ms
Average fpcount: 7.260000 ± 3.161076
```