# restrander

A program for restranding Fastq files.

Parses an input `.fq` file, classifying each record as either `forward`, `reverse` or `ambiguous`, and outputs to a new `.fq` with the classification of each read included in a `direction` tag. For reverse reads, the reverse-complement of the original sequence is recorded, and quality scores are also reversed. 

# Usage instructions

Simply run `restrander`, giving one input file and one output file:

```
./restrander input.fq output.fq
```

Optionally, you can include another parameter to select the restranding method:

```
./restrander input.fq output.fq restranding_method
```

- **standard** attempts PolyA/PolyT classification, and if this returns ambiguous, it applies SSP/VNP classification.
- **slow** does the same, but allows for higher edit distance in SSP/VNP classification.
- **superslow** does the same, but allows for even higher edit distance in SSP/VNP.
- **trimmed** is for trimmed reads, which are in the form ``` mRNA - polyA ``` or ``` polyT - mRNA ```. Since they have no SSP/VNP, this method only applies PolyA/PolyT classification.

# How it works

Each forward read is assumed to have a particular form, based on Oxford Nanopore PCR-cDNA PCB109 design:

``` barcode - SSP - mRNA - polyA - VNP_reverse_complement - barcode_reverse_complement ```

While a reverse read takes the form:

``` barcode - VNP - polyT - mRNA_reverse_complement - SSP_reverse_complement - barcode_reverse_complement ```

Because of these differences in form, we can use a few techniques for classifying direction:

## PolyA/PolyT Classification

A naive method of looking through each sequence for a PolyA tail of ~12 consecutive As near the end (indicative of a positive strand read), or a PolyT leader of ~12 consecutive Ts near the start (indicative of a negative strand read).

| PolyA tail present  | PolyT leader present  | Classification  |
| ------------------- | ------------------- | --------------- |
| Yes                 | No                  | Forward         |
| No                  | Yes                 | Reverse         |
| Yes                 | Yes                 | Ambiguous       |
| No                  | No                  | Ambiguous       |

The drawback of this method is that often sequences include native PolyA/PolyT stretches, leading to ambiguous reads.

## SSP/VNP Classification

A method of searching for the SSP and VNP near the start of the read to classify it. Often the SSP and VNP are not perfectly present in the read, so we allow for some [Levenshtein edit distance](https://en.wikipedia.org/wiki/Levenshtein_distance).

| SSP found in sequence | VNP found in sequence | Classification  |
| --------------------- | --------------------- | --------------- |
| Yes                   | No                    | Forward         |
| No                    | Yes                   | Reverse         |
| Yes                   | Yes                   | Ambiguous       |
| No                    | No                    | Ambiguous       |

This method takes notably longer than PolyA/PolyT classification. Additionally, sometimes the SSP and VNP are so wrong in the sequence that we cannot identify them, leading to ambiguous reads.
