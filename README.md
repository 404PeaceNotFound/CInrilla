# Equipe8

Em desenvolvimento...

arquitetura:
Equipe8 (futuro CInrilla)/
├── Makefile
├── assets/                 # Sprites, Sons, Fontes
└── src/
    ├── main.c              # Inicialização e Loop Principal apenas
    │
    ├── core/               # Módulos essenciais (Independem do jogo)
    │   ├── window.c        # Gerenciamento de Janela
    │   ├── input.c         # Abstração de Teclado/Mouse
    │   ├── time.c          # Delta time (dt) e controle de frame
    │   └── audio_core.c    # Engine de áudio (não a lógica de som do jogo)
    │
    ├── data/               # Definição de estruturas de dados puras (Headers)
    │   ├── entity_types.h  # Structs de Player, Enemy (sem lógica)
    │   └── map_data.h      # Structs do mapa
    │
    ├── entities/           # "Factories" e gestão de estado
    │   ├── player.c        # Inicialização e dados específicos do player
    │   ├── enemies.c       # Gestão de lista de inimigos
    │   └── items.c         # Consumíveis
    │
    ├── systems/            # A LÓGICA pesada (Processam as Entities)
    │   ├── physics.c       # Colisões e Movimento (trata player e enemies igual)
    │   ├── combat.c        # Ataque e Vida (lógica de dano)
    │   ├── render.c        # Desenha o mapa e entidades (inclui Câmera)
    │   └── audio_sys.c     # Toca sons baseados em eventos
    │
    ├── scenes/             # Gerenciamento de Estados do Jogo
    │   ├── scene_manager.c # Troca entre Menu -> Game -> GameOver
    │   ├── menu.c
    │   ├── gameplay.c      # Onde o jogo roda (une Systems e Entities)
    │   └── credits.c
    │
    ├── ui/                 # Interface do Usuário (Sobreposta ao jogo)
    │   ├── hud.c           # Barra de vida, pontuação (Lê dados, não altera)
    │   └── widgets.c       # Botões, Caixas de texto
    │
    └── utils/              # Funções auxiliares genéricas
        ├── math_utils.c    # Vetores, Matrizes
        └── event_bus.c     # SISTEMA CRÍTICO para desacoplamento