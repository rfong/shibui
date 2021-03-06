# shibui

`shibui` is my 5x3+3 [QMK](https://qmk.fm/) keymap for a `crkbd` board with:
- a trackpoint hotswap mod on the right-hand side
- a 128x32 OLED on the left-hand side
- 5V Pro Micro controllers with `qmk-dfu` bootloaders

Todolist at [TODO.md](TODO.md).

# Keymap ergonomics

This 36-key layout is designed to alleviate my primary RSI issue, which is pinky nerve pain, and to use as few layers as possible. You will probably not enjoy this keymap if your primary RSI issues are in your thumbs. For more details on design choices, layout, and layer use cases, see [KEYMAP.md](KEYMAP.md).

If my keymap design decisions do not appeal to you, you might prefer the popular [`miryoku`](https://github.com/manna-harbour/miryoku) layout.

# Hardware build logs

- [R61 trackpoint keyboard mod, part 1](https://rfong.github.io/rflog/2021/10/18/r61-trackpoint-pt1/): sourcing & disassembling the module. No real procedural changes from @manna-harbour's [crkbd hotswap trackpoint doc](https://github.com/manna-harbour/crkbd/blob/master/trackpoint/readme.org), but lots of pictures & tips.
- [R61 trackpoint keyboard mod, part 2](https://rfong.github.io/rflog/2021/10/26/r61-trackpoint-pt2/): physical assembly, mounting, wiring. My build works in tight spaces and is rearranged to fit over keyboard controllers that use headers instead of low profile pins.
- For related writeups including my keymap ergonomics design, ISP reflashing guide, and keycap dyeing, see the [#keyboard tag](https://rfong.github.io/rflog/tag/keyboard/) on my blog.

# Using with QMK

With the exception of some top level files, this follows the directory structure of QMK. Custom firmware code is defined in `keyboards/crkbd/keymaps/rfong`.

## Setup

For a quick setup, soft link my QMK keymap folder into your local `qmk_firmware` folder.
```
QMK_HOME=`qmk config -ro user.qmk_home | cut -d= -f2`
ln -s keyboards/crkbd/keymaps/rfong $QMK_HOME/keyboards/crkbd/keymaps/rfong
```
(Can also `git submodule`, but I find that annoying for personal development.)

## Build firmware

(from within `qmk_firmware`)

**Compile/flash right**
```
sudo make crkbd:rfong:dfu-split-right MH_MODULE=trackpoint
```

**Compile/flash left**
```
sudo make crkbd:rfong:dfu-split-left
```

# Using with VIA (just the keymap)

Including my VIA keymap file for posterity at [crkbd_via_shibui.json](master/tree/crkbd_via_shibui.json). The trackpoint will not work, but the Mod-Tap thumbs and other macros will work fine.

---

# Credit

- Thanks manna-harbour for writing the first [QMK trackpoint hotswap doc](https://github.com/manna-harbour/crkbd/blob/master/trackpoint/readme.org) and very excellent firmware support for it! Most of my PS2 handling configuration is borrowed from the `crkbd:manna-harbour` keymap.
- Thanks tzarc and fauxpark for helping me figure out my broken bootloaders!
- Most of the OLED configuration comes from `crkbd:default`.
