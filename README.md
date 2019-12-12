# precise-time

A small executable to measure startup time and maximum memory usage precisely.

```
$ precise-time 10 `which ruby` -e ':hello'
48 47 46 46 47 46 47 47 47 46
47 [46-48] ms
14 [14-14] MB
```

Compare this to the traditional `time` command:
```
$ (Zsh) time ruby -e ':hello'
ruby -e ':hello'  0.05s user 0.01s system 98% cpu 0.056 total

$ (Bash) time ruby -e ':hello'
real    0m0.058s
user    0m0.047s
sys     0m0.011s
```

Or to `/usr/bin/time`:
```
$ /usr/bin/time `which ruby` -e ':hello'
0.05user 0.00system 0:00.05elapsed 98%CPU (0avgtext+0avgdata 14344maxresident)k
0inputs+0outputs (0major+2397minor)pagefaults 0swaps

$ /usr/bin/time -v `which ruby` -e ':hello'
        User time (seconds): 0.04
        System time (seconds): 0.00
        Percent of CPU this job got: 96%
        Elapsed (wall clock) time (h:mm:ss or m:ss): 0:00.05
        Maximum resident set size (kbytes): 14372
        ...
```

None of those gives the real startup time or an estimation of variance.

The `time` command and `/usr/bin/time` also have different precision and output
per platform, which makes them impractical to compare on different platforms.

## Compile

```
make
```

## Install

Install to `~/bin`:
```
$ make install
```

Install the executable under the given path:
```
make install DEST=/my/prefix/bin
```

## Alternatives

* [hyperfine](https://github.com/sharkdp/hyperfine) is a similar but much more fancy command-line benchmarking tool.
