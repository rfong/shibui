# keymap design & rationale

For the actual keymap, see the `keymaps` constant array in [`keymap.c`](https://github.com/rfong/shibui/blob/main/keyboards/crkbd/keymaps/rfong/keymap.c).

This 36-key layout is designed to alleviate my primary RSI issue, which is pinky nerve pain, and to use as few layers as possible. You will probably not enjoy this keymap if your primary RSI issues are in your thumbs. If my design decisions do not appeal to you, you may prefer the popular [`miryoku`](https://github.com/manna-harbour/miryoku) layout.

### Design priorities
- Reallocates traditional pinky work to the thumbs, in particular all modifiers
- Leverage existing muscle memory from standard laptop keyboard layouts, but compactified
- Chording comfort and minimal layer-switching for keys frequently used together
- Use cases: `vim`, terminal ergonomics, notetaking
- Numrow instead of numpad

### Implementation choices: finger usage
- The pinky is never required to access modifiers or special functions.
- All modifiers and most special functions are accessible through the thumb cluster.
- I chose not to add symmetric `Shift`s or `Space`s because I did [a typing test](https://monkeytype.com/) and found that I preferred to access them from only one hand. Your mileage may vary.

### Implementation choices: chording
- A chord may only require two keys in the same thumb cluster *if no other keys are used on that hand*. This allows two fingers to comfortably move down to the thumb cluster without contorting the rest of the hand. A chord may *never* require *three* keys in the same thumb cluster.
- All symbols are accessible via either a one-handed chord, or a two-key chord. This allows them to be rapidly typed in sequence.
- Some redundancies are built in to simplify common shortcuts and use cases. For example, `Enter` and `Esc` can be accessed from either hand.

### Other implementation choices
- I currently use momentary layers rather than toggles (or "oneshot" layers). This may change in the future.

-----

Currently, this is only formatted for import into VIA.

### Main layers

- Layer 0 (base layer) is mostly for letters.

Layers 1-3 are momentary, activated by holding the layer keys in the thumb cluster.
- Layer 1 is for numbers (in row layout), navigation, and symbols.
- Layer 2 RH is dedicated to one-hand chords convenient for coding.
- Layer 2 LH contains macros, shortcuts, and redundant modifier access.
- Layer 3 contains some extras where typing speed is not particularly critical.

![layout](https://github.com/rfong/shibui/blob/main/layout.jpg)

### Special layers

- Layer 4 contains mousebutton keys. It is momentarily activated while trackpoint activity is detected.

Layers 5-6 are toggled by doubletapping layer keys.
- Layer 5 is intended for math symbols. I have not populated it yet.
- Layer 6 is intended as a dedicated navigation layer. I have mostly not populated it yet.

### Thumb clusters

Thumb cluster behavior is the same on all layers. Mod-tap and tapdance are used to maximize functionality.

|          | L outer | L mid    | L inner | R inner | R mid    | R outer |
| ---      | ---     | ---      | ---     | ---     | ---      | ---     |
| **HOLD** | `Ctrl`  | `Layer1` | `Cmd`   | `Shift` | `Layer2` | `Alt`   |
| **TAP**  | `Tab`   |          | `Enter` | `Space` |          | `Esc`   |
| **2TAP** |         | `Layer5` |         |         | `Layer6` |         |

Should your RSI and physical thumbs differ from mine, you may wish to reorder your thumb cluster accordingly. I used the following rationale:
- Assign the `Layer` modifiers to whichever key you feel most comfortable holding down while accessing a range of other keys.
- Assign your most commonly used modifiers & specials to whichever key feels best to tap quickly.
- Assign your least used modifiers & specials to your least favorite key.

### Deliberate redundancies

- `Esc` can be used either as a single RH thumb tap or LH 2-key chord.
- `Enter` can be used either as a single LH thumb tap or RH 2-key chord.
- Layer 2 has an auxiliary `Shift` to enable shifted RH symbol chording without putting double duty on the RH thumb. In addition, some shifted index finger keys on this layer are broken out to avoid awkward chords and provide easy access to `|` and `:`.
- The `()` on Layer 2 is there to keep all braces clustered near each other on the same layer for coding convenience. 
- `-`/`_` on Layer 2 is for coding and arithmetic. The `-`/`_` on Layer 1 is convenient for dates, phone numbers, filenames, and other numeric sequences that are frequently interspersed with hyphens and underscores. Both are useful to minimize layer switching in different cases.
