# Models-of-Galaxies-and-Clusters
# 🌌 Galaxy Models Simulation – Plummer & Hernquist

A C-based simulation that models the mass distribution of galaxies using the **Plummer** and **Hernquist** density profiles.  
This project calculates **density**, **mass profile**, and **circular velocity** for different radius values and compares the nature of both galaxy models.

---

## ✨ Features

- Simulates two astrophysical galaxy models:
  - **Plummer Model** – Smooth, core-like star cluster
  - **Hernquist Model** – Galaxy-like, highly concentrated center
- Numerical integration using **Simpson’s Rule**
- Mass Profile & Circular Velocity computation
- Modular and scalable C architecture
- Accepts user inputs for custom simulation runs

---

## 🧠 Models Overview

| Model | Best Represents | Key Behavior |
|-------|------------------|----------------|
| **Plummer** | Globular Clusters | Smooth central core, gentle fall-off |
| **Hernquist** | Galaxies & Dark Matter Halos | Sharp central concentration, realistic galaxy structure |

---

## 🧩 Tech Stack

- **Language:** C
- **Build Tool:** `Makefile`
- **Compiler:** GCC

---
## 🚀 Getting Started

### ✅ Clone the Repository

```bash
git clone https://github.com/Nayan993/Models-of-Galaxies-and-Clusters.git
cd plummer-hernquist-galaxy-models
```

### 🧱 Build the Project

```bash
make
```

### ▶️ Run the Simulation

```bash
 make run
```

---

## 📥 User Inputs

When the program runs, it will ask for:

1. **Start Radius**
2. **End Radius**
3. **Step Size**
4. **Choice of Model**  
   - Enter `1` or `plummer` for Plummer model  
   - Enter `2` or `hernquist` for Hernquist model  

---

## 🧪 Example Run

```
Enter start radius: 0
Enter end radius: 10
Enter step size: 0.5
Choose model (1/2 or plummer/hernquist): plummer
```

---

## 📤 Sample Output

```
Radius      Density         Mass         Velocity
0.0         1.000000        0.000000     0.000000
0.5         0.853430        0.253212     1.012459
1.0         0.500000        0.984121     1.421657
...
```

For Hernquist, the mass rises sharply near the center compared to Plummer.

---

## 📂 Project Structure

```
├── main.c                      # Program controller
├── sampling.c / sampling.h     # Generates radius samples
├── density.c / density.h       # Plummer & Hernquist density formulas
├── integration.c / integration.h # Simpson's Rule for mass integration
├── profiles.c / profiles.h     # Mass & circular velocity profiles
├── utils.c / utils.h           # File handling & helper functions
├── Makefile
└── README.md
```

---

## 🔄 Program Flow (How It Works)

```
Main.c → Sampling → Density → Integration → Profiles → Utils → Output
```

---

## 👥 Contributors

| Name | Reg No | Contribution |
|-------|-----------|----------------|
| **Nayan Mishra** | 2025CA062 | Team Lead, main.c, Sampling |
| **Anuj Sahu** | 2025CA017 | Plummer Density Model |
| **Sonu Kumar** | 2025CA098 | Hernquist Density Model |
| **Anshi Tiwari** | 2025CA016 | Simpson's Integration Logic |
| **Komal Kushwaha** | 2025CA051 | Integration Optimization & Validation |
| **Vineet Kumar** | 2025CA110 | Mass & Velocity Profile Calculations |
| **Ajayveer Chauhan** | 2025CA009 | Output Storage & File Writing |
| **Abhishek Kapoor** | 2025CA002 | Utility Functions |
| **Himanshu Thakur** | 2025CA105 | Utils & Support |
| **Pragati Singh** | 2025CA070 | Documentation, PPT, README |
| **Harshit Raj** | 2025CA041 | PPT, Documentation, README Formatting |

---

## ⚠️ License

This project is currently **not under any open-source license**.  
All rights reserved.  
(Anyone cannot reuse code without permission.)

---

## ⭐ Future Enhancements

- Auto-plot graphs using Python/gnuplot
- Add N-Body simulation support
- 3D visualization
