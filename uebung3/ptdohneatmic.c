#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <string.h>



int main() {
    FILE *file = fopen("../uebung2/gefilterte_wetterdaten.csv", "r");

    if (file == NULL) {
        printf("Fehler beim Öffnen der Datei!\n");
        return 1;
    }

    int count = 0;
    char line[256];
    double *temperatures = NULL;
    int capacity = 0;

    /* Header-Zeile ueberspringen */
    if (fgets(line, sizeof(line), file) == NULL) {
        printf("Datei ist leer.\n");
        fclose(file);
        return 1;
    }

 
    {
        while (fgets(line, sizeof(line), file) != NULL) {
        int jahr, monat, tag, stunde, minute;
        double temperatur;

        if (sscanf(line,
                   "%d,%d,%d,%d,%d,%lf",
                   &jahr,
                   &monat,
                   &tag,
                   &stunde,
                   &minute,
                   &temperatur) == 6) {
            if (count == capacity) {
                int new_capacity = capacity == 0 ? 128 : capacity * 2;
                double *new_temperatures = realloc(temperatures, (size_t)new_capacity * sizeof(double));

                if (new_temperatures == NULL) {
                    printf("Speicher konnte nicht reserviert werden.\n");
                    free(temperatures);
                    fclose(file);
                    return 1;
                }

                temperatures = new_temperatures;
                capacity = new_capacity;
            }

            temperatures[count] = temperatur;
            count++;
        }
    }
    } 


    fclose(file);

    if (count == 0) {
        printf("Keine gueltigen Temperaturwerte gefunden.\n");
        return 1;
    }


      int count = 1000;
    float temperatures[1000];

    // Beispielwerte
    for (int i = 0; i < count; i++) temperatures[i] = (i % 80) - 40; // Werte von -40 bis 39

    int counts[8];
    double sum;

    for (int iteration = 0; iteration < 1000; iteration++) {
        sum = 0.0;
        memset(counts, 0, sizeof(counts));

        #pragma omp parallel
        {
            int counts_private[8] = {0};  // Jeder Thread hat eigenes Zählarray
            double sum_private = 0.0;

            #pragma omp for
            for (int i = 0; i < count; i++) {
                sum_private += temperatures[i];
                for (int n = 0; n < 8; n++) {
                    float lower = (n - 3) * 10;
                    float upper = lower + 10;
                    if (temperatures[i] >= lower && temperatures[i] < upper) {
                        counts_private[n]++;  // Thread-sicher, da privat
                    }
                }
            }

            // Zusammenführen in das globale Array
            #pragma omp critical
            {
                sum += sum_private;
                for (int n = 0; n < 8; n++) {
                    counts[n] += counts_private[n];
                }
            }
        }

        // Ausgabe
        // printf("Iteration %d: Sum=%.3f, Durchschnitt=%.3f\n", iteration + 1, sum, sum / count);
        for (int n = 0; n < 8; n++) {
            printf("Iteration %d: Anzahl Werte im Bereich [%d, %d): %d\n",
                   iteration + 1, (n - 3) * 10, (n - 2) * 10, counts[n]);
        }
    }

   

    printf("Anzahl Werte: %d\n", count);
    printf("Summe Temperaturen: %.3f\n", sum);
    printf("Durchschnittliche Temperatur: %.3f\n", sum / count);

    free(temperatures);

    return 0;
}