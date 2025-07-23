# ==============================================================================
# Dockerfile para Ambiente de Desenvolvimento C++/Emscripten (VERSÃO CORRIGIDA)
# ==============================================================================

# 1. Imagem Base (usando uma tag de versão específica)
FROM emscripten/emsdk:3.1.60

# ------------------------------------------------------------------------------
# 2. Criação de Usuário e Instalação de Ferramentas
# ------------------------------------------------------------------------------
#
# Executa todos os comandos de configuração em uma única camada 'RUN' para
# otimizar e garantir a ordem correta.
#
RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    # Instala o 'sudo' para dar permissões ao nosso novo usuário
    && apt-get install -y --no-install-recommends sudo \
    # Cria um grupo chamado 'vscode' com ID de grupo (gid) 1001
    && groupadd --gid 1001 vscode \
    # Cria um usuário chamado 'vscode' com ID (uid) 1001 e o associa ao grupo 'vscode'
    && useradd --uid 1001 --gid 1001 -m vscode \
    # Dá ao usuário 'vscode' permissão para usar 'sudo' sem precisar de senha
    && echo "vscode ALL=(ALL) NOPASSWD:ALL" > /etc/sudoers.d/vscode \
    # Instala as outras ferramentas que precisamos
    && apt-get install -y --no-install-recommends \
       git \
       cmake \
       ninja-build \
       vim \
    # Limpa o cache do apt para manter a imagem pequena
    && apt-get autoremove -y && apt-get clean -y && rm -rf /var/lib/apt/lists/*

COPY ./.bashrc /home/vscode/.bashrc
# ------------------------------------------------------------------------------
# 3. Define o usuário 'vscode' como o usuário padrão para o restante da imagem
#    e para quando o contêiner for iniciado.
# ------------------------------------------------------------------------------
USER vscode
WORKDIR /workspaces