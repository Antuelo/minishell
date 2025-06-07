#!/bin/bash

TEST_FILE="tests/test_cases.txt"
MINISHELL_BIN="./minishell"
EXPECTED_DIR="tests/expected"
INDEX=1

while IFS= read -r line; do
	echo "🧪 Test $INDEX : $line"

	# Exécute la commande dans ton minishell et enregistre la sortie
	echo "$line" | $MINISHELL_BIN > tests/output_$INDEX.txt

	# Compare avec le fichier attendu
	EXPECTED_FILE="$EXPECTED_DIR/test_$INDEX.txt"

	if [ ! -f "$EXPECTED_FILE" ]; then
		echo "⚠️ Fichier attendu manquant : $EXPECTED_FILE"
	else
		diff tests/output_$INDEX.txt "$EXPECTED_FILE" > /dev/null
		if [ $? -eq 0 ]; then
			echo "✅ Test $INDEX OK"
		else
			echo "❌ Test $INDEX FAILED"
			echo "Différence :"
			diff tests/output_$INDEX.txt "$EXPECTED_FILE"
		fi
	fi

	echo "---------------------"
	((INDEX++))
done < "$TEST_FILE"
