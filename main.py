import time
import sys
import pandas as pd
import matplotlib.pyplot as plt
from wordcloud import WordCloud

CSV_PATH = "output/trendingTopics.csv"

if len(sys.argv) > 1:
    REFRESH_SECONDS = int(sys.argv[1])
else:
    REFRESH_SECONDS = 1

def generate_wordcloud():
    try:
        df = pd.read_csv(CSV_PATH)

        freq_dict = dict(zip(df['topic'], df['frequency']))

        wc = WordCloud(
            width=800,
            height=600,
            background_color="white"
        ).generate_from_frequencies(freq_dict)

        plt.clf()
        plt.imshow(wc, interpolation="bilinear")
        plt.axis("off")
        plt.title("Trending Topics (actualizado)")
        plt.pause(0.01)

    except Exception as e:
        print("ERROR procesando CSV:", e)

def main():
    plt.ion()
    print("Generando WordCloud en vivo...")
    while True:
        generate_wordcloud()
        time.sleep(REFRESH_SECONDS)

if __name__ == "__main__":
    main()