# EmbarcaBell: Sistema de Monitoramento de Ruído Hospitalar com Indicação Visual em LEDs

## Descrição do Projeto

O sistema é composto por um microfone de alta sensibilidade que capta os sons do ambiente e os envia para um microcontrolador Raspberry Pi Pico. Esse microcontrolador converte os sinais analógicos do microfone em valores digitais via ADC, calcula o nível de ruído em decibéis (dB) e controla a intensidade de LEDs RGB via PWM.

Os LEDs mudam de cor de acordo com o nível de ruído:
- Azul: Ambiente silencioso.
- Roxo: Nível de ruído moderado.
- Vermelho: Ruído excessivo.

Além disso, é utilizado um filtro exponencial para suavizar a transição entre os níveis de brilho, evitando piscadas indesejadas nos LEDs.

## Estrutura do Projeto

```
PROJETO/
├── .vscode/            # Configurações do VS Code (opcional)
├── build/              # Diretório de build (gerado pelo CMake)
├── .gitignore          # Arquivo para ignorar arquivos no controle de versão
├── CMakeLists.txt      # Configuração do CMake para compilação do projeto
├── pico_sdk_import.cmake # Importação do SDK do Raspberry Pi Pico
├── projeto.c               # Código principal do projeto
```

## Dependências
Para compilar e rodar este projeto, você precisará de:
- Raspberry Pi Pico SDK
- CMake
- Um ambiente de desenvolvimento C (GCC ARM)

## Compilação e Execução
1. Configure o ambiente do SDK do Raspberry Pi Pico.
2. No terminal, navegue até o diretório do projeto:
   ```sh
   git clone https://github.com/HiagoMCarvalho/EmbarcaBell.git
   cd ADC
   ```
3. Crie um diretório de build e entre nele:
   ```sh
   mkdir build && cd build
   ```
4. Execute o CMake para configurar o projeto:
   ```sh
   cmake ..
   ```
5. Compile o projeto:
   ```sh
   make
   ```
6. Envie o arquivo `.uf2` gerado para o Raspberry Pi Pico.


## Funcionamento do Código
- O microfone capta ruídos e manda para o Raspberry pi pico w que por sua vez utiliza ADC para convertes os sinais analógicos do microfone em sinais digitais
- Os leds vermelho e azul acompanham os valores recebidos do microfone e alteram suas intensidades através de pwm
- Quanto maior for o nível de ruído recebido, menor será a intensidade do led azul e maior será a intensidade do led vermelho

## Autor
Desenvolvido por <https://github.com/HiagoMCarvalho>

## Vídeo
Assista ao vídeo explicação: <https://drive.google.com/file/d/1S8PorWgUbPg6vVyC--fqhRJ8Kc6PdXiN/view?usp=sharing>

## Licença
Este projeto está sob a licença MIT.

