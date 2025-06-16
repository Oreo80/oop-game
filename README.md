# Undertale:: Ribbit Edition
O recreere a sistemului de luptă din Undertale, 
realizată folosind C++ și SFML. Proiectul reproduce 
mecanicile de luptă turn-based ale jocului original, 
permițând jucătorilor să aleagă între acțiunile clasice—Fight, 
Act, Item și Mercy—și să navigheze printr-un meniu de acțiuni. 
Sistemul de luptă include și mecanica specifică bullet hell, 
unde jucătorii trebuie să evite atacurile inamicilor într-un mod 
interactiv.

### Instrucțiuni
---
### 🐸 Deplasare 🐸
- W sau ⬆️ pentru a te mișca în sus
- D sau ➡️ pentru a te mișca în dreapta
- A sau ⬅️ pentru a te mișca în stânga
- S sau ⬇️ pentru a te mișca în jos
---
### 👾 Meniu 👾
- F4 pentru fullscreen
- Enter sau Z pentru confirmarea alegerii
- Shift sau X pentru anularea alegerii
- ESC pentru închiderea ferestrei
---
### 🎯 Condiții de câștig 🎯
Pentru a învinge inamicul Froggit, jucătorii pot alege una dintre următoarele strategii:
- 🟢 Pacifist: Folosește acțiunea Compliment sau Threat, apoi Spare din meniul Mercy.
- 🟡 Mercy sub presiune: Așteaptă ca Froggit să ajungă la o viață scăzută (< 10 HP) și alege Spare.
- 🔴 Neutral: Învinge Froggit folosind acțiunea Fight până când HP-ul lui ajunge la 0.
---

## Resurse
- [UndertaleModTool](https://github.com/UnderminersTeam/UndertaleModTool)
- [nlohmann/json](https://github.com/nlohmann/json) (MIT)
- [SFML](https://github.com/SFML/SFML/tree/2.6.1) (Zlib)
  - [OpenAL](https://openal-soft.org/) (LGPL): din cauza licenței, trebuie distribuită ca shared library
