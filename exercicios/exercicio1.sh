#!/bin/bash


# 1. Como procurar por uma palavra no manual (man)?
man bash

# 2. Liste todos os arquivos do seu diretório HOME.
ls ~

# 3. Como você pode saber em qual diretório você está no sistema?
pwd

# 4. Qual o comando para saber qual a conta que está sendo usada em um SHELL?
whoami

# 5. Liste o nome do grupo de usuários ao qual sua conta pertence.
groups

# 6. Visite alguns diretórios, alternando os comandos pwd e cd. Os diretórios são os seguintes:
  # /home/espinf
  # o predecessor do diretório /home/espinf
  # /
  # /usr
  # /tmp
  # /usr/bin
  # seu diretório HOME

  cd /home/espinf
  cd ..
  cd /
  cd usr
  cd ../tmp
  cd /usr/bin
  cd ~

# 7. Liste o conteúdo dos diretórios acima, ordenados por data, de duas formas:
  # sem sair do seu diretório HOME
dirs="/home/espinf / /usr /tmp /usr/bin ~"
ls -lut $dirs

# acessando primeiramente o diretório a ser listado
for dir in $dirs
do
  cd $dir
  ls -lut
done

# 8. O que faz a opção -a no comando ls? E a opção -l?
  # A opção -a do comando ls mostra todos os arquivos do diretório corrente,
  # inclusive arquivos ponto, que, sem essa opção, permanecem ocultos.

# 9. Liste usuários do sistema com o mesmo nome que o seu
  # Não é possível ter dois usuários com o mesmo nome no Unix.

# 10. O que são arquivos/diretórios cujo nome inicia com um caractere . ?
  # Arquivos que iniciam com um ponto são arquivos ocultos de uma listagem padrão.
  # Podem ser usados para limpar um pouco o diretório, ocultando alguns arquivos de
  # configuração que não são necessários de se ver o tempo todo, como o .bashrc
  # Podem ser usados também para tornar invisível a uma listagem padrão feita por
  # um programador não muito precavido e evitar a remoção, por exemplo, de alguns
  # arquivos críticos, ou pelo menos importantes, em algum diretório.

# 11. Crie um link simbólico para o diretório infraComp do seu colega ao lado. Use este link para listar o conteúdo do diretório
ln -s /home/colega-home/infraComp colegaAoLado
ls colegaAoLado


# 12. O que fazem os comandos du e df?
