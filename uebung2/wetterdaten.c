#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    FILE *file = fopen("02_Gatow-1_2025_Daten.csv", "r");
    FILE *out = fopen("gefilterte_wetterdaten.csv", "w");

    if (file == NULL || out == NULL) {
        printf("Fehler beim Öffnen der Datei!\n");
        if (file != NULL) {
            fclose(file);
        }
        if (out != NULL) {
            fclose(out);
        }
        return 1;
    }

    char line[1024];
    int in_data_section = 0;
    int written_lines = 0;

    fprintf(out, "Jahr,Monat,Tag,Stunde,Minute,air_temperature\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        if (!in_data_section) {
            if (strstr(line, "# begin of data") != NULL) {
                in_data_section = 1;
            }
            continue;
        }

        int jahr, monat, tag, stunde, minute;
        int wday, woy, doy;
        double rel_humidity, air_temp_2m, air_temp_5cm, precipitation;
        char q_humidity[2], q_air_temp_2m[2], q_air_temp_5cm[2], q_precipitation[2];

        int parsed = sscanf(line,
                            "%d%d%d%d%d%d%d%d%lf %1s %lf %1s %lf %1s %lf %1s",
                            &jahr,
                            &monat,
                            &tag,
                            &stunde,
                            &minute,
                            &wday,
                            &woy,
                            &doy,
                            &rel_humidity,
                            q_humidity,
                            &air_temp_2m,
                            q_air_temp_2m,
                            &air_temp_5cm,
                            q_air_temp_5cm,
                            &precipitation,
                            q_precipitation);

        if (parsed != 16) {
            continue;
        }

        if (q_air_temp_5cm[0] == 'g' || q_air_temp_5cm[0] == 'k' || q_air_temp_5cm[0] == 'q') {
            fprintf(out, "%d,%02d,%02d,%02d,%02d,%.3f\n",
                    jahr,
                    monat,
                    tag,
                    stunde,
                    minute,
                    air_temp_2m);
            written_lines++;
        }
    }

    fclose(file);
    fclose(out);
    printf("Fertig: %d Zeilen nach gefilterte_wetterdaten.csv geschrieben.\n", written_lines);
    return 0;
}