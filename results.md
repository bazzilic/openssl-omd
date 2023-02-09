```
                                                         |              INTEL                 |                 M1                 |
    CURVE   | PubKey Length | Total B/clue | Total Bytes | Detection time | Detection/message | Detection time | Detection/message |
 secp224r1  |   29 bytes    |   45 bytes   |  22.5 MiB   |   30'654 ms    |    0.0613 ms      |   24'926 ms    |    0.0613 ms      |
 sect113r1  |   16 bytes    |   32 bytes   |  16.0 MiB   |   43'876 ms    |    0.0878 ms      |   34'352 ms    |    0.0878 ms      |
 sect113r2  |   16 bytes    |   32 bytes   |  16.0 MiB   |   43'759 ms    |    0.0875 ms      |   34'407 ms    |    0.0875 ms      |
 prime256v1 |   33 bytes    |   49 bytes   |  24.5 MiB   |   25'742 ms    |    0.0515 ms      |   20'611 ms    |    0.0515 ms      |
```

## Intel CPU (3rd Gen Intel Xeon Ice Lake with AVX-512) – M6i AWS Instance

```
BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_secp224r1 curve
Public key length: 29 bytes
keygen done in 33.794000 ms
msggen done in 30873.852000 ms
6 non-pertinent messages were mistakenly considered pertinent
detect done in 30654.769000 ms
 TOTAL done in 61562.472000 ms

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r1 curve
Public key length: 16 bytes
keygen done in 98.669000 ms
msggen done in 44076.187000 ms
7 non-pertinent messages were mistakenly considered pertinent
detect done in 43876.273000 ms
 TOTAL done in 88051.179000 ms

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r2 curve
Public key length: 16 bytes
keygen done in 94.618000 ms
msggen done in 43868.577000 ms
7 non-pertinent messages were mistakenly considered pertinent
detect done in 43759.466000 ms
 TOTAL done in 87722.706000 ms

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_X9_62_prime256v1 curve
Public key length: 33 bytes
keygen done in 19.129000 ms
msggen done in 25833.072000 ms
2 non-pertinent messages were mistakenly considered pertinent
detect done in 25742.170000 ms
 TOTAL done in 51594.416000 ms
```

 ## M1 Max Macbook Pro ARM64 CPU

```
 BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_secp224r1 curve
Public key length: 29 bytes
keygen done in 27.144000 ms
msggen done in 25443.105000 ms
2 non-pertinent messages were mistakenly considered pertinent
detect done in 25283.865000 ms
 TOTAL done in 50754.169000 ms

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r1 curve
Public key length: 16 bytes
keygen done in 73.990000 ms
msggen done in 34683.205000 ms
13 non-pertinent messages were mistakenly considered pertinent
detect done in 34352.184000 ms
 TOTAL done in 69109.425000 ms

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_sect113r2 curve
Public key length: 16 bytes
keygen done in 72.496000 ms
msggen done in 34305.121000 ms
13 non-pertinent messages were mistakenly considered pertinent
detect done in 34407.827000 ms
 TOTAL done in 68785.485000 ms

BENCHMARKING   1000 agents; 500000 msgs; 100 pertinent   using   NID_X9_62_prime256v1 curve
Public key length: 33 bytes
keygen done in 14.863000 ms
msggen done in 20791.275000 ms
7 non-pertinent messages were mistakenly considered pertinent
detect done in 20611.341000 ms
 TOTAL done in 41417.511000 ms
```