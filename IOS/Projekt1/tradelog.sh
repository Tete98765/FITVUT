#!/bin/sh

export POSIXLY_CORRECT=yes
export LC_NUMERIC=en_US.UTF-8
export LC_COLLATE=C

print_help() {
    echo "Usage: tradelog [-hl--help]"
    echo "        tradelog [FILTER...] [COMMAND] [LOG...]"
    echo "  list-tick       -print list of all tickers "
    echo "  profit          -print profit from all closed tickets"
    echo "  pos             -print list of values of all currently held positions sorted descending by value"
    echo "  last-price      -print list of last currently known prices for tickers"
    echo "  hist-ord        -histograms of number of transactions by tickers"
    echo "  graph-pos       -graph of values of currently held positions by tickers"
    echo ""
    echo "FILTERS (.......)"
    echo "  -a DATETIME     -after: only records AFTER this date are considered (without input date). "
    echo "                          The format if a DATETIME is YYYY-MM-DD HH:MM:SS."
    echo "  -b DATETIME     -before: only records BEFORE this date are considered (without input date)."
    echo "                           The format if a DATETIME is YYYY-MM-DD HH:MM:SS."
    echo "  -t              -only entries corresponding to a given ticker are considered"
    echo "  -w              -sets the WIDTH of printed graphs, i.e. sets the length of the longest row to given number"
    echo "                  -WIDTH has to be a whole number >= 1"

}
#načítavanie príkazov 
while [ "$#" -gt 0 ]; do
    case $1 in
    list-tick | profit | pos | last-price | hist-ord | graph-pos)
        COMMAND="$1"
        shift
        ;;
    -a)
        DATE_AFTER="$2"
        shift
        shift
        ;;
    -b)
        DATE_BEFORE="$2"
        shift
        shift
        ;;
    -t)
        if [ -z "$TICKERS" ]; then
            TICKERS="$2"
        else
            TICKERS="$TICKERS|$2"
        fi
        shift
        shift
        ;;
    -w)
    #keď sa vo w niečo už nachádza, tak vypíšem nápovedu a ukončím program
        if [ -n "$WIDTH" ]; then
            print_help
            exit 1
        fi
        WIDTH="$2"
        shift
        shift
        ;;
    -h | --help)
        print_help
        exit 0
        ;;
    *)
        if [ -f "$1" ]; then
        #rozlišovanie typu súboru 
            if echo "$1" | grep -q -P "\.gz$"; then
                GZ_FILES="$1 $GZ_FILES"
            else
                FILES="$1 $FILES"
            fi
        fi
        shift
        ;;
    esac
done

if [ -n "$GZ_FILES" ]; then
    # pokiaľ existujú GZ súbory, prečítajú sa a pošlú sa do catu s plaintext files
    LOG=$(gzip -d -c $GZ_FILES | cat $FILES -)
else
    if [ -n "$FILES" ]; then
        LOG=$(cat $FILES)
    else
        LOG=$(cat -)
    fi
fi

if [ -n "$TICKERS" ]; then
    LOG=$(
        echo "$LOG" |
            awk \
                -F ';' \
                -v "t=^($TICKERS)$" \
                '{
                    if ($2 ~ t) {
                        print
                    }
                }'
    )
fi

if [ -n "$DATE_AFTER" ]; then
    LOG=$(
        echo "$LOG" |
            awk \
                -F ';' \
                -v "da=$DATE_AFTER" \
                '{
                    if (da < $1) {
                        print
                    }
                }'
    )
fi

if [ -n "$DATE_BEFORE" ]; then
    LOG=$(
        echo "$LOG" |
            awk \
                -F ';' \
                -v "db=$DATE_BEFORE" \
                '{
                    if (db > $1) {
                        print
                    }
                }'
    )
fi

if [ "$COMMAND" = "list-tick" ]; then
    echo "$LOG" |
        awk \
            -F ";" \
            '{
                print $2
            }' |
        sort | uniq
elif [ "$COMMAND" = "profit" ]; then
    echo "$LOG" |
        awk \
            -F ";" \
            '{ 
                if ($3 == "sell") {
                    sell+=$4 * $6
                }
                if ($3 == "buy") {
                    buy+=$4 * $6
                }
            } END {
                  prof=sell-buy
                  printf("%.2f\n",prof)
            }'
elif [ "$COMMAND" = "pos" ]; then
    echo "$LOG" |
        awk \
            -F ";" \
            '{ 
                if ($3 == "sell") {
                    tick_count[$2]-=$6
                }
                if ($3 == "buy") {
                    tick_count[$2]+=$6
                }

                tick_last_price[$2]=$4
            } END {
                most_length = 0
                #sortujem a hľadám najväčšiu hodnotu tickeru 
                for (tick in tick_count) {
                    len = length(sprintf("%.2f", tick_count[tick] * tick_last_price[tick]))
                    if (most_length < len) {
                        most_length = len
                    }
                }
                #úprava formátu pre správne vytlačenie
                format = sprintf("%%-10s: %%%s.2f\n", most_length)

                for (tick in tick_count) {
                    printf(format, tick, tick_count[tick] * tick_last_price[tick])
                }
            }' |
        sort -nr -k2 -t:
elif [ "$COMMAND" = "last-price" ]; then
    echo "$LOG" |
        awk \
            -F ";" \
            '{
                tick_last_price[$2]=$4
            } END {
                most_length = 0
                for (tick in tick_last_price) {
                    len = length(sprintf("%.2f", tick_last_price[tick]))
                    if (most_length < len) {
                        most_length = len
                    }
              }

              format = sprintf("%%-10s: %%%s.2f\n", most_length)

              for (tick in tick_last_price) {
                  printf(format, tick, tick_last_price[tick])
              }
            }' | sort -t: -k1
elif [ "$COMMAND" = "hist-ord" ]; then
    echo "$LOG" |
        awk \
            -F ";" \
            -v "w=$WIDTH" \
            '{
                transactions[$2]+=1
            } END {

                #pri zadadom width zistím  najväčšiu hodnotu a podľa nej zistím počet dielov na jednu #
                if (w > 0) {
                    most_transactions = 0
                    for (tick in transactions) {
                        if (most_transactions < transactions[tick]) {
                            most_transactions = transactions[tick]
                        }
                    }

                  char_count = most_transactions / w
                }

                for (tick in transactions) {
                    printf "%-10s:", tick
                    if (w > 0) {
                       chars = int(transactions[tick] / char_count)
                    }
                    else {
                        chars = transactions[tick]
                    }

                    if (transactions[tick] > 0 && chars > 0) {
                        printf " "

                        for (i=0; i < chars; i++) {
                            printf "#"
                        }
                    }
                    printf "\n"
                }
            }' | sort -t: -k1
elif [ "$COMMAND" = "graph-pos" ]; then
    echo "$LOG" |
        awk \
            -F ";" \
            -v "w=$WIDTH" \
            '
            function abs(v) {
                if (v < 0) {
                    v = -v
                }
                return v
            }

            {
                if ($3 == "sell") {
                    tick_count[$2]-=$6
                }
                if ($3 == "buy") {
                    tick_count[$2]+=$6
                }
              tick_last_price[$2]=$4
            } END {
                if (w > 0) {
                    max_value = 0
                    for (tick in tick_count) {
                        #zaokrúhlenie kvôli záporným hodnotám
                        v = abs(tick_count[tick] * tick_last_price[tick])
                        if (max_value < v) {
                          max_value = v
                        }
                    }

                    count_to_char = max_value / w

                    for (tick in tick_count) {
                        printf "%-10s:", tick

                        value = tick_count[tick] * tick_last_price[tick]
                        chars = abs(int(value / count_to_char))

                        if (value > 0) {
                            char = "#"
                        }
                        else {
                            char = "!"
                        }

                        if (chars > 0) {
                            printf " "
                            for (i=0; i < chars; i++) {
                                printf "%s", char
                            }
                        }
                        printf "\n"
                    }
                } else {
                    for (tick in tick_count) {
                        printf "%-10s:", tick

                        value = tick_count[tick] * tick_last_price[tick]
                        if (value > 0) {
                           char="#"
                        }
                        else {
                            char="!"
                        }

                        chars = abs(int(value / 1000))

                        if (chars > 0) {
                            for (i=0; i < chars; i++) {
                                printf "%s", char
                            }
                        }

                        printf "\n"
                    }
                }
            }' | sort -t: -k1
else
    echo "$LOG"
fi
