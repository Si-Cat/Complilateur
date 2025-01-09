# Complilateur
# Analyseur Syntaxique et Sémantique pour un Langage de Programmation Fictif

Dans le cadre de ce projet, nous avons développé un analyseur syntaxique et sémantique pour un langage de programmation fictif. Ce projet vise à renforcer la compréhension des concepts fondamentaux liés à la compilation, tels que l’analyse lexicale, l’analyse syntaxique, et l’analyse sémantique.

## Table des Matières
- [Objectifs](#objectifs)
- [Structure du Projet](#structure-du-projet)
- [Fonctionnalités Implémentées](#fonctionnalités-implémentées)
- [Exemple de Grammaire Utilisée](#exemple-de-grammaire-utilisée)
- [Exemple d’Exécution](#exemple-dexécution)
- [Résultats et Analyse](#résultats-et-analyse)
- [Conclusion](#conclusion)

## Objectifs

- Implémenter un analyseur syntaxique basé sur une grammaire donnée.
- Vérifier la validité des instructions et expressions du programme source.
- Générer un fichier de traduction contenant une représentation intermédiaire du programme.
- Détecter et signaler les erreurs sémantiques, telles que l’utilisation de variables non déclarées ou les incompatibilités de types.

## Structure du Projet

Le projet est structuré en plusieurs étapes :

1. **Analyse Lexicale** : Extraction des tokens (identificateurs, mots-clés, opérateurs, etc.).
2. **Analyse Syntaxique** : Vérification de la structure du programme à l’aide de règles syntaxiques définies.
3. **Analyse Sémantique** : Validation des types et vérification des déclarations de variables.
4. **Génération de Traduction** : Création d’un fichier contenant une représentation intermédiaire du programme.

## Fonctionnalités Implémentées

- Gestion des instructions conditionnelles (`if`, `else`).
- Boucles (`while`).
- Lecture et écriture (`read`, `write`, `writeln`).
- Détection et gestion des erreurs sémantiques :
  - Variables non déclarées.
  - Incompatibilités de types dans les affectations.

## Exemple de Grammaire Utilisée

```plaintext
1  PROGRAM ID ; Dcl InstComposee .
2  Dcl VAR ID : TYPE ;
3  InstComposee BEGIN I ; END
4  I Exp
      ID := Exp
      | IF Exp THEN I [ELSE I]
      | WHILE Exp DO I
      | READ(ID)
      | WRITE(ID)
5  Exp_simple [RELOP Exp_simple]
6  Exp_simple Terme {ADDOP Terme}
7  Terme Facteur {MULOP Facteur}
8  Facteur ID | NB | CHAR_LITERAL | (Exp)
```

## Exemple d’Exécution

### Fichier source d’entrée (`test.txt`)

```plaintext
1 PROGRAM Exemple;
2 VAR x : INTEGER;
3 BEGIN
4   x := 10;
5   IF x > 5 THEN
6     WRITE(x)
7   ELSE
8     WRITELN(x);
9 END.
```

## Résultats et Analyse

- Le programme est capable de détecter des erreurs syntaxiques et sémantiques avec des messages clairs.
- Le fichier de traduction est généré correctement pour des programmes valides.

## Conclusion

Ce mini-projet nous a permis d’explorer les étapes fondamentales de la création d’un compilateur. Nous avons appris à structurer un analyseur syntaxique, gérer les erreurs sémantiques, et générer une traduction intermédiaire.


