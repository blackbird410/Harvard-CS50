-- Keep a log of any SQL queries you execute as you solve the mystery.
-- First let's look at the crime scene reports
SELECT description FROM crime_scene_reports
WHERE day = 28 AND month = 07 AND year = 2021 AND street = 'Humphrey Street';
-- We found that the theft took place at 10:15 AM at the Humphrey Street bakery. There were three witnesses who mentionned the bakery.

-- Let's look at those interviews
SELECT transcript FROM interviews WHERE day = 28 AND month = 07 AND year = 2021 AND transcript LIKE '%bakery%';
-- We now know that the theft took place within 10 minutes, the thief got into a car in the bakery parking lot and drive away.
-- Also, earlier in the morning, the thief withdrew some money at the ATM at Leggett Street.
-- Lastly, while leaving the thief called someone and talked for less than one minute. They said they were planning to take the earliest
-- flight out of Fiftyville tomorrow. His accomplice would buy the tickets.

--Now, let's look at the bakery security logs and try to find the license plate of the car the thief left in between 10:15 AM - 10:25 AM
SELECT activity, license_plate FROM bakery_security_logs
WHERE day = 28
AND month = 07
AND year = 2021
AND hour = 10
AND (minute BETWEEN 15 AND 25);
-- We found 8 license plate leaving the crime scene between the time frame.

-- Let's see who those car plates belongs to.
SELECT DISTINCT(people.id), name, passport_number, people.license_plate FROM people
JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
WHERE people.license_plate in (
    SELECT license_plate FROM bakery_security_logs
    WHERE day = 28
    AND month = 07
    AND year = 2021
    AND hour = 10
    AND (minute BETWEEN 15 AND 25)
);
-- They belong to : Vanessa, Barry, Iman, Sofia, Luca, Diana, Kelsey, Bruce

-- Now let's see which one of those persons withdrew money at the ATM at Leggett Street in the morning of the theft.
SELECT name FROM people
JOIN bank_accounts ON people.id = bank_accounts.person_id
JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
WHERE atm_location = 'Leggett Street'
AND transaction_type = 'withdraw'
AND day = 28
AND month = 07
AND year = 2021
AND name in (
    SELECT DISTINCT(name) FROM people
    JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
    WHERE people.license_plate in (
        SELECT license_plate FROM bakery_security_logs
        WHERE day = 28
        AND month = 07
        AND year = 2021
        AND hour = 10
        AND (minute BETWEEN 15 AND 25)
    )
);
-- We found that four of those person withdrew money at this ATM in the day of the crime, they are : Bruce, Diana, Iman and Luca.

-- Now let's see who made a call between 10:15 AM and 10:25 AM for less than 1 minute.
SELECT name FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
WHERE duration < 60
AND year = 2021
AND month = 07
AND day = 28
AND name in (
    SELECT name FROM people
    JOIN bank_accounts ON people.id = bank_accounts.person_id
    JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
    WHERE atm_location = 'Leggett Street'
    AND transaction_type = 'withdraw'
    AND day = 28
    AND month = 07
    AND year = 2021
    AND name in (
        SELECT DISTINCT(name) FROM people
        JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
        WHERE people.license_plate in (
            SELECT license_plate FROM bakery_security_logs
            WHERE day = 28
            AND month = 07
            AND year = 2021
            AND hour = 10
            AND (minute BETWEEN 15 AND 25)
        )
    )
);
-- We found that on this day, only Bruce and Diana have had calls during the theft and for less than 1 minute.

-- Now let's see which one of them is taking a plane out of Fiftyville on July 29 on the earliest flight
SELECT name, hour, minute, day, month, a1.city AS origin, a2.city AS destination FROM people
JOIN passengers ON people.passport_number = passengers.passport_number
JOIN flights ON passengers.flight_id = flights.id
INNER JOIN airports a1 ON flights.origin_airport_id = a1.id
INNER JOIN airports a2 ON flights.destination_airport_id = a2.id
WHERE a1.city = 'Fiftyville'
AND year = 2021
AND month = 07
AND day = 29
AND name in (
    SELECT name FROM people
    JOIN phone_calls ON people.phone_number = phone_calls.caller
    WHERE duration < 60
    AND year = 2021
    AND month = 07
    AND day = 28
    AND name in (
        SELECT name FROM people
        JOIN bank_accounts ON people.id = bank_accounts.person_id
        JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
        WHERE atm_location = 'Leggett Street'
        AND transaction_type = 'withdraw'
        AND day = 28
        AND month = 07
        AND year = 2021
        AND name in (
            SELECT DISTINCT(name) FROM people
            JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
            WHERE people.license_plate in (
                SELECT license_plate FROM bakery_security_logs
                WHERE day = 28
                AND month = 07
                AND year = 2021
                AND hour = 10
                AND (minute BETWEEN 15 AND 25)
            )
        )
    )
)
ORDER BY hour, minute
LIMIT 1;
-- It was Bruce who left for New York City at 8:20 AM.

-- Let's find his accomplice's number with the call he made
SELECT receiver FROM people
JOIN phone_calls ON people.phone_number = phone_calls.caller
AND year = 2021
AND month = 07
AND day = 28
AND duration < 60
WHERE caller = (SELECT phone_number FROM people WHERE name = (
    SELECT name FROM people
    JOIN passengers ON people.passport_number = passengers.passport_number
    JOIN flights ON passengers.flight_id = flights.id
    INNER JOIN airports a1 ON flights.origin_airport_id = a1.id
    INNER JOIN airports a2 ON flights.destination_airport_id = a2.id
    WHERE a1.city = 'Fiftyville'
    AND year = 2021
    AND month = 07
    AND day = 29
    AND name in (
        SELECT name FROM people
        JOIN phone_calls ON people.phone_number = phone_calls.caller
        WHERE duration < 60
        AND year = 2021
        AND month = 07
        AND day = 28
        AND name in (
            SELECT name FROM people
            JOIN bank_accounts ON people.id = bank_accounts.person_id
            JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
            WHERE atm_location = 'Leggett Street'
            AND transaction_type = 'withdraw'
            AND day = 28
            AND month = 07
            AND year = 2021
            AND name in (
                SELECT DISTINCT(name) FROM people
                JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
                WHERE people.license_plate in (
                    SELECT license_plate FROM bakery_security_logs
                    WHERE day = 28
                    AND month = 07
                    AND year = 2021
                    AND hour = 10
                    AND (minute BETWEEN 15 AND 25)
                )
            )
        )
    )
    ORDER BY hour, minute
    LIMIT 1
));

-- Finally, let's find the name of the accomplice
SELECT name FROM people WHERE phone_number = (
    SELECT receiver FROM people
    JOIN phone_calls ON people.phone_number = phone_calls.caller
    AND year = 2021
    AND month = 07
    AND day = 28
    AND duration < 60
    WHERE caller = (SELECT phone_number FROM people WHERE name = (
        SELECT name FROM people
        JOIN passengers ON people.passport_number = passengers.passport_number
        JOIN flights ON passengers.flight_id = flights.id
        INNER JOIN airports a1 ON flights.origin_airport_id = a1.id
        INNER JOIN airports a2 ON flights.destination_airport_id = a2.id
        WHERE a1.city = 'Fiftyville'
        AND year = 2021
        AND month = 07
        AND day = 29
        AND name in (
            SELECT name FROM people
            JOIN phone_calls ON people.phone_number = phone_calls.caller
            WHERE duration < 60
            AND year = 2021
            AND month = 07
            AND day = 28
            AND name in (
                SELECT name FROM people
                JOIN bank_accounts ON people.id = bank_accounts.person_id
                JOIN atm_transactions ON bank_accounts.account_number = atm_transactions.account_number
                WHERE atm_location = 'Leggett Street'
                AND transaction_type = 'withdraw'
                AND day = 28
                AND month = 07
                AND year = 2021
                AND name in (
                    SELECT DISTINCT(name) FROM people
                    JOIN bakery_security_logs ON people.license_plate = bakery_security_logs.license_plate
                    WHERE people.license_plate in (
                        SELECT license_plate FROM bakery_security_logs
                        WHERE day = 28
                        AND month = 07
                        AND year = 2021
                        AND hour = 10
                        AND (minute BETWEEN 15 AND 25)
                    )
                )
            )
        )
        ORDER BY hour, minute
        LIMIT 1
    ))
);
-- And the mysterious accomplice, ladies and genteman, is: Robin

