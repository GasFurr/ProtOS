<p align="center">
  <img src="https://github.com/user-attachments/assets/e597ff5e-c87b-4fc6-ba28-3a1fcadbf761" alt="ProtOS Logo" width="1920">
</p>

<h3 align="center">🛠 Rethink the Wheel | 🐾 Paws-on Development | 🔵 Blueprint for Modern Computing</h3>

<div align="center">
  
  [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
  ![GitHub commit activity](https://img.shields.io/github/commit-activity/m/GasFurr/ProtOS)
  ![Version](https://img.shields.io/badge/Version-0.1.9--alpha-red)
  [![Build](https://img.shields.io/badge/Build-Passing-brightgreen)](https://github.com/GasFurr/ProtOS/actions)

</div>

---

> **⚠️ Dragon Warning**  
> ProtOS is currently in its embryonic stage. Expect broken code, sudden rewrites, and occasional tears. Not suitable for production (or sanity).

## 🚀 Philosophy in Three Acts

<div align="center">

**⚡ Fast & Fresh**

*Modern hardware deserves modern software - no legacy baggage allowed*

**🧩 Modular by Design**

*Just snap components together, all in one system configuration file.*

**🔭 Future-First**

*Why walk when you can teleport? No legacy baggage, robust GRUB-powered foundation*

</div>

---

## 📦 Project Anatomy

```bash
ProtOS/
├── build/          # Compiled objects
├── docs/           # Documentation womb
├── grub/           # Bootloader configs
├── headers/        # C headers party
├── src/            # Kernel guts
│   ├── boot/       # Boot sequence acrobatics
│   ├── drivers/    # Hardware whisperers
│   └── kernel/     # Core brain surgery
└── tools/          # Build-time gadgets
```

## 🛠️ Building the Beast

### Summon dependencies (choose your distro flavor)

<details>
<summary>Arch/Manjaro (yay required)</summary>

```bash
# Summon compiler dragons (i use arch btw)
yay -S grub nasm gcc binutils qemu-full edk2-ovmf
```

</details>

<details>
<summary>Debian/Ubuntu (apt required)</summary>

```bash
# For our Debian/Ubuntu friends:
sudo apt install grub-pc nasm gcc binutils qemu-system-x86 ovmf
```

</details>

```bash
# Clone with surgical precision
git clone https://github.com/GasFurr/ProtOS.git

# Summon the build spirits
make all

# For brave souls only:
make run

# To bring tears
make debug

# To just build the iso
make release 
```

---

> **🐉 Dragon's Note**  
> QEMU testing requires OVMF firmware. We don't ship it for the same reason
> dragons don't share gold: Lawyers. Get yours from
> [EDK2](https://github.com/tianocore/edk2) or `sudo pacman -S edk2-ovmf`
> then copy `OVMF_CODE.fd` to firmware folder. No hoard, no boot!

---

## 📋 Progress Report (Survival Mode)

| Task                          | Status    | Notes                |
|-------------------------------|-----------|----------------------|
| boot.asm                      | 🚧 80%    | Assembler yoga       |
| Basic makefile                | ✅ 100%   | Builds without fire  |
| grub.cfg                      | ✅ 100%   | Bootloader ready     |
| MB2 tags header               | 🚧 40%    | Multiboot mysteries  |
| IDT & GDT                     | 🚫 0%     | CPU gatekeeping      |
| Graphics/text mode drivers    | 🚧 80%    | Pixel uwusability    |
| Keyboard interwupts           | 🚫 0%     | Keypress archaeology |
| Time drivers                  | 🚫 0%     | Chronomancy          |
| bschell                       | 🚫 0%     | Terminal therapy     |
| Branding                      | ✅ 100%   | No longer disgusting |

---

## 💥 ALPHA 0.1.9.1 CHANGELOG

  Just works. Little patch

### 🚀 New features

- Better logging.
- 64KB stack

### ⚡ Optimization

- No changes

### 🔧 Fixed

- make debug just not worked.
- Now it works.

### 📝 Documentation

- No changes

### 🐉 Known issues

- No changes

---

## 🔮 Crystal Ball (Roadmap)

- **Phase 1**: Make computer go beep (hardware communication)
- **Phase 2**: Pretend we're UNIX (basic shell operations)
- **Phase 3**: ??? (profit)

---

## 📜 License to Tinker

MIT Licensed - Do whatever you want, but maybe mention us if it blows up.  
*"Rethink the Wheel"* isn't just a slogan - it's a way of life (and possibly yarn).

<p align="left">
  <img src="https://github.com/user-attachments/assets/77f70569-2c2f-444d-b800-6cd5005d2e5d" alt="ProtOS Logo" width="600">
</p>

---

<div align="center">
  
## 📬 Contact

Got questions, suggestions, or dragon taming tips?  
📧 [gasfurri@gmail.com](mailto:gasfurri@gmail.com)
  
  ![GitHub last commit](https://img.shields.io/github/last-commit/GasFurr/ProtOS?color=blue)

</div>

---

```asciiart
 /\_/\  
( o.o ) 
 > ^ <  /* Compiles with love */
```

"UNIX is user-friendly. It’s just selective about who its friends are"
(carved on a kernel developer's tombstone)
