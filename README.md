# Adreno650 (FFT-infra fork)

A650 kernel GPU driver for the Xiaomi Redmi K40 (`alioth`, SM8250-AC).

This repository forks [UniXNU/Adreno740](https://github.com/UniXNU/Adreno740).
The upstream driver targets A740 on SM8550 and is named for that component;
this fork carries the A650 (a6xx family) path that SM8250 boards need.

| Repository | Contents |
| --- | --- |
| [UniXNU/Adreno740](https://github.com/UniXNU/Adreno740) | upstream: A740 on SM8550, verified on sheng |
| [FFT-infra/Adreno650](https://github.com/FFT-infra/Adreno650) | this fork: A650 on SM8250, the `fft` branch |

## What the fork changes

- Reads `pipa-gpu-pool` rather than `sheng-gpu-pool` (`bdee54c`). The board
  package passes the former, so before this the pool argument never reached
  the allocator and every pool experiment measured nothing.
- Keeps two client text lines per process (`d7bfe27`). The change had been
  sitting uncommitted in the working tree through every image from u252 to
  u270.

The driver keeps upstream's component identity: it is selected as
`Adreno740` in the catalog and built as `build/adreno740/libAdreno740.a`,
because `scripts/build-xnu-4k-baseline.sh` hard-codes that archive name and
ignores `CONFIG_XNU_NATIVE_ADRENO_ARCHIVE`. The component name describes the
catalog entry, not the silicon; this board's silicon is an A650.

## State on this board

GMU cold start, SMMU routing, the CP's first command, accelerator publish and
the staged Metal self-test all pass. The desktop draws. What fails is the
client IOVA window: `slot0` spans 512–1536 MiB and `slot1` 1536–1706 MiB,
while SpringBoard peaks at 1727 MiB, so it loses its Metal device and refuses
buffer and texture allocations with `OutOfMemory(Device)`.

## Build

Selected from the device repository's `feeds.toml` as the `Adreno740` source,
and compiled by `scripts/build-adreno650.py` in the device repository. Read
[Adreno740 documentation](https://unixnu.com/drivers/adreno740),
[GPU porting](https://unixnu.com/zh/porting/gpu), the
[component catalog](https://unixnu.com/reference/components) and
[`AGENTS.md`](AGENTS.md).

Device page: <https://unixnu.com/zh/devices/alioth>.
Porting record: [`../../UniXNU-alioth/porting.toml`](../../UniXNU-alioth/porting.toml).
