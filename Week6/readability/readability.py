from cs50 import get_string


def count_letters(text):
    letters = 0
    l = len(text)
    for i in range(l):
        # Check if the current character is a letter
        if text[i].isalpha():
            letters += 1
    return letters


def count_words(text):
    return len(text.split())


def count_sentences(text):
    s = 0
    for x in text:
        if x in ["!", ".", "?"]:
            s += 1
    return s


def main():
    # Take the user input
    text = get_string("Text: ")

    # Count the number of letters, words and sentences
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Coleman-Liau formula: index = 0.0588 * L - 0.296 * S - 15.8
    l = (letters / words) * 100
    s = (sentences / words) * 100
    index = round(0.0588 * l - 0.296 * s - 15.8)

    if index >= 16:
        print("Grade 16+")
    elif index < 1:
        print("Before Grade 1")
    else:
        print(f"Grade {index}")


if __name__ == "__main__":
    main()

