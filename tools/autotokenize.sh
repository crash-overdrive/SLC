ls  ../marmoset | xargs -n 1 basename | cut -d '.' -f1 | xargs -I {} sh -c "python ../../../tools/tokenizer.py ../../../build/lib/tokens.lex ../marmoset/{}.java > {}.tokens"
