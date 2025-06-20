#!/bin/bash

MINISHELL=./minishell
BASH_TESTER=bash
TMP_DIR=test/
mkdir -p $TMP_DIR

# Función para testear un comando y comparar salidas
run_test() {
    local cmd="$1"

    echo -e "\n🔹 Test: $cmd"

    echo "$cmd" | $MINISHELL > $TMP_DIR/minishell_out.txt 2> $TMP_DIR/minishell_err.txt
    STATUS_MINI=$?

    echo "$cmd" | $BASH_TESTER > $TMP_DIR/bash_out.txt 2> $TMP_DIR/bash_err.txt
    STATUS_BASH=$?

    echo "stdout:"
    diff -u $TMP_DIR/bash_out.txt $TMP_DIR/minishell_out.txt || echo "❌ Diferencia en stdout"
    echo "stderr:"
    diff -u $TMP_DIR/bash_err.txt $TMP_DIR/minishell_err.txt || echo "❌ Diferencia en stderr"
    if [ "$STATUS_MINI" -ne "$STATUS_BASH" ]; then
        echo "❌ Exit status diferente: minishell=$STATUS_MINI | bash=$STATUS_BASH"
    else
        echo "✅ Exit status OK: $STATUS_MINI"
    fi
}

# Lista de comandos a testear
TESTS=(
  'echo hola mundo'
  'echo -n sin_salto'
  'export VAR1=valor1'
  'echo $VAR1'
  'unset VAR1'
  'echo $VAR1'
  'cd /tmp'
  'pwd'
  'cd -'
  'exit 42'
  'echo hello'
  'echo "hello world"'
  "echo 'hello world'"
  'echo "hello   world"'
  "echo 'user: \$USER'"
  'echo "user: \$USER"'
  'echo "$USER said '\''hello'\''"'
  'echo "chemin: \$HOME"'
  'echo "quote: \"hello\""'
  'echo "antislash: \\test"'
  "echo 'antislash: \\test'"
  'ls > out.txt'
  'ls >> out.txt'
  'cat < in.txt'
  'cat < in.txt > out.txt'
  'cat < in.txt >> out.txt'
  'cat << EOF'
  'ls | grep main'
  'echo "coucou" | cat -e'
  'echo "un" | echo "deux" | echo "trois"'
  'ls > out.txt | grep .c >> result.txt'
  'echo > out.txt > out2.txt'
  'echo coucou < in.txt < in2.txt'
  'cat <'
  'cat <<'
  'echo coucou >'
  'echo coucou >>'
  'ls >>> file'
  'cat <<<< EOF'
  'echo "quote ouverte"'
  "echo 'quote ouverte"
  'echo bonjour $UNDEFINED_VAR'
  'echo $?'
  '| ls'
  'ls |'
  'ls || echo coucou'
  'ls ||| echo fail'
  'cat << |'
  'cat << >'
  'echo Coucou && echo Salut'
  'echo ouch &&& echo help'
  'ls |< grep test'
  'ls | < grep test'
  'cat <<< EOF'
  'echo Hello~'
  'with\ space'
  'escaped\>char'
  'echo\$HOME'
  'no\escape\'
  'trailing\'
  'double\\backslash'
  'echo first > out1.txt | grep second | wc -l > out2.txt'
  'echo test > file1 file2'
  'cat << EOF | grep hello'
  'echo "$UNDEFINED"'
  'echo hello '\''>'\'' file'
)


# Ejecutar los tests
for test in "${TESTS[@]}"; do
    run_test "$test"
done

rm -rf $TMP_DIR
