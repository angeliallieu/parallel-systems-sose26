#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *file = fopen("gefilterte_wetterdaten.csv", "r");

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

    fclose(file);

    if (count == 0) {
        printf("Keine gueltigen Temperaturwerte gefunden.\n");
        return 1;
    }

    double sum = 0.0;
    for (int i = 0; i < count; i++) {
        sum += temperatures[i];
    }

    printf("Anzahl Werte: %d\n", count);
    printf("Summe Temperaturen: %.3f\n", sum);
    printf("Durchschnittliche Temperatur: %.3f\n", sum / count);

    free(temperatures);

    return 0;
}