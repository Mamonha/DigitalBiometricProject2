# DigitalBiometricProject
# Atenção isso não é uma versão definitiva. 
Sistema de Coleta de Biometria Digital com implementação básica de CRUD, com registro de usuário associado a biometria.

# Como funciona?
No arquivo main você encontra toda a funcionalidade do sistema, em suma o microprocessador ESP8266 se conecta com uma rede wifi e posteriormente se conecta com uma base de dados. O sistema possui um registro de usuário pela serial da arduino IDE, digitando 1 realiza um break que coleta os dados de usuário e uma biometria(Sensor biométrico).
Posteriormente ocorre a inserção de dados na base de dados criada.

# Componentes:
Leitor Biométrico DY-50;
Wemos Lolin D1-R2(Esp8266);
LCD 16x2.

# Limitações:
Leitor biométrico possui uma limitação de 127 biometrias, para desenvolvimento de aplicações comerciais é necessário armazenar em outro dispositivo ou utilizar outro método.
O microprocessador Esp8266 possui algumas limitações de desempenho, para desenvolvimento de aplicações maiores recomendo o Esp32 ou superior.
Obviamente como se trata de um sistema integrado a web, é necessário conexão com a internet.

# Upgrades:
O sistema atualmente realiza a inserção diretamente no banco de dados, consequentemente isso a fins profissionais se torna algo inseguro. Deste modo é necessário a realização de um Back-End, para tratar essas requisições. Como melhoria de desempenho a implementação de um Sensor biométrico melhor e principalmente um ESP-32 melhoria o fluxo de funcionamento do sistema.

# Contato:
Mais informações sobre o projeto no linkedin.
Tem alguma duvida, alguma proposta de desenvolvimento similar ao apresentado ou proposta profissional?, entre em contato.
E-mail: andersoncardoso302@hotmail.com
Discord: Mamonha#9320.
Linkedin: https://www.linkedin.com/in/anderson-cardoso-da-silva-579216122/.
