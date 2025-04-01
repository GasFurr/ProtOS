<p align="center">
  <img src="https://github.com/user-attachments/assets/5f8269bf-997f-4cbc-92c1-9cda7ad919ec" alt="ProtOS Logo" width="800">
</p>

<h3 align="center">🛠 Rethink the Wheel | 🐾 Paws-on Development | 🔵 Blueprint for Modern Computing</h3>

<div align="center">
  
  [![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](https://opensource.org/licenses/MIT)
  ![GitHub commit activity](https://img.shields.io/github/commit-activity/m/GasFurr/ProtOS)
  ![Version](https://img.shields.io/badge/Version-0.1.3--alpha-red)
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
*Like LEGO for operating systems - snap components together*

**🔭 Future-First**  
*Why walk when you can teleport? UEFI-only, GRUB-powered foundation*

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
```

---

> **🐉 Dragon's Note**  
> QEMU testing requires OVMF firmware. We don't ship it for the same reason
> dragons don't share gold: Lawyers. Get yours from 
> [EDK2](https://github.com/tianocore/edk2) or `sudo pacman -S edk2-ovmf`
> then copy `OVMF_CODE.fd` to firmware folder. No hoard, no boot!

---

## 📋 Progress Report (Survival Mode)

| Task                          | Status    | Notes               |
|-------------------------------|-----------|---------------------|
| boot.asm                      | 🚫 0%     | Assembler yoga      |
| Basic makefile                | ✅ 100%   | Builds without fire |
| grub.cfg                      | ✅ 100%   | Bootloader ready    |
| MB2 tags parser               | 🚫 0%     | Multiboot mysteries |
| IDT & GDT                     | 🚫 0%     | CPU gatekeeping     |
| Graphics/text mode drivers    | 🚫 0%     | Pixel uwusability   |
| Keyboard interwupts           | 🚫 0%     | Keypress archaeology|
| Time drivers                  | 🚫 0%     | Chronomancy         |
| bschell                       | 🚫 0%     | Terminal therapy    |
| branding                      | 🚧 70%    | Working on logo     |

---

## 🔮 Crystal Ball (Roadmap)
- **Phase 1**: Make computer go beep (hardware communication)
- **Phase 2**: Pretend we're UNIX (basic shell operations)
- **Phase 3**: ??? (profit)

---

## 📜 License to Tinker
MIT Licensed - Do whatever you want, but maybe mention us if it blows up.  
*"Rethink the Wheel"* isn't just a slogan - it's a way of life (and possibly yarn).

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
