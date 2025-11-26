# 🧱 Arkanoid C Project - CESAR School

Projeto final da disciplina de **Programação Imperativa e Funcional (2025.2)**.
Um clone moderno do clássico **Arkanoid** desenvolvido em C puro utilizando a biblioteca **Raylib**.

O jogo apresenta mecânicas avançadas como sistema de física, partículas, múltiplos níveis, skins personalizáveis e power-ups variados.

---

## 👥 Equipe
* **Joao Arthur Gomes Duarte**
* **Luiza**
* **Maria Augusta**

---

## 🎮 Funcionalidades do Jogo

### 🌟 Destaques
* **3 Fases Diferentes:** Layouts "Clássico", "Pilares" e "A Caixa" (escolha aleatória ao vencer).
* **Sistema de Vidas nos Blocos:**
    * 🟥 **Vermelho:** 3 Hits (Duro)
    * 🟧 **Laranja:** 2 Hits (Médio)
    * 🟩 **Verde:** 1 Hit (Fraco)
* **Personalização:**
    * **23 Skins** de bola diferentes.
    * **Backgrounds Dinâmicos** que mudam conforme a skin escolhida.
* **Sistema de High Score:** Salva seu recorde localmente em arquivo binário.
* **Juice & Game Feel:** Sistema de partículas, efeito de "shake" e visual 3D nos blocos.

### ⚡ Power-Ups
Os power-ups caem aleatoriamente ao destruir blocos:
1. 🍄 **Aumentar:** Aumenta o tamanho da raquete.
2. 💀 **Diminuir:** Diminui o tamanho da raquete (Cuidado!).
3. 🐢 **Slow Motion:** Deixa a bola lenta temporariamente para facilitar.
4. 🔄 **Inverter:** Inverte os controles Esquerda/Direita por 5 segundos.
5. 🔫 **Arma:** Equipa a raquete com canhões para atirar nos blocos.
6. ✖️ **Duplicar:** Duplica a quantidade de bolas na tela.

---

## 🛠️ Pré-requisitos e Instalação

Este projeto foi desenvolvido para rodar em ambiente **Linux** ou **WSL (Windows Subsystem for Linux)**.

### 1. Instalar Dependências (Obrigatório)
Antes de compilar, você precisa instalar as bibliotecas gráficas necessárias para o Raylib:

```bash
sudo apt update
sudo apt install build-essential git cmake libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
git clone [https://github.com/luizabalmeida/ARKANOID-GAME.git](https://github.com/luizabalmeida/ARKANOID-GAME.git)
cd ARKANOID-GAME
make
./arkanoid
```
## 🕹️ Como Jogar

| Tecla | Ação |
| :--- | :--- |
| **Setas (⬅️ / ➡️)** | Movem a raquete para a esquerda e direita. |
| **Espaço** | Atira (se tiver o Power-Up de Arma) ou Inicia o jogo. |
| **Enter** | Confirma seleção nos menus. |

---

## 📂 Estrutura do Projeto

* **`main.c`**: Código fonte principal (Game Loop, Lógica, Renderização).
* **`game_structs.h`**: Definição das estruturas de dados (Structs).
* **`Makefile`**: Script de automação de compilação.
* **`skins/`**: Imagens das skins da bola.
* **`backgrounds/`**: Imagens de fundo dos níveis.
* **`img/`**: Ícones dos Power-Ups.
* **`scores.dat`**: Arquivo binário onde o recorde é salvo (gerado automaticamente).

---

## 📺 Demonstração
