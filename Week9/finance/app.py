import os

from cs50 import SQL
from flask import Flask, flash, redirect, render_template, request, session
from flask_session import Session
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

from datetime import datetime

# Configure application
app = Flask(__name__)

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")


@app.after_request
def after_request(response):
    """Ensure responses aren't cached"""
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    # Querying the stocks owned by the user
    stocks = db.execute(
        "SELECT symbol, shares FROM stocks WHERE user_id=? GROUP BY symbol;",
        session["user_id"],
    )

    total_stocks = 0

    # Checking the stocks prices
    for stock in stocks:
        s = lookup(stock["symbol"])
        stock["name"] = s["name"]
        stock["price"] = usd(s["price"])
        stock["total"] = usd(stock["shares"] * s["price"])
        total_stocks += stock["shares"] * s["price"]

    # Querying the cash available for the user
    cash = db.execute("SELECT cash FROM users WHERE id=?;", session["user_id"])[0][
        "cash"
    ]

    # Grand total
    g_total = cash + total_stocks

    return render_template(
        "index.html",
        stocks=stocks,
        total_stocks=usd(total_stocks),
        cash=usd(cash),
        g_total=usd(g_total),
    )


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    if request.method == "GET":
        return render_template("buy.html")
    else:
        # Check the user's input
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("must enter a symbol", 400)
        elif not shares:
            return apology("must enter a positive number", 400)
        elif not shares.isnumeric():
            return apology("must enter a positive number", 400)
        elif int(shares) < 0:
            return apology("must enter a positive number", 400)
        else:
            quote = lookup(symbol)
            if not quote:
                return apology("must enter a valid symbol", 400)
            else:
                # Querying the cash available for the user
                cash = db.execute(
                    "SELECT cash FROM users WHERE id=?;", session["user_id"]
                )[0]["cash"]

                # Check if the user can afford the amount of shares queried
                if int(shares) * quote["price"] > cash:
                    return apology(
                        "must enter an affordable amount of shares, cash unsufficient",
                        400,
                    )
                else:
                    # Proceed to purchase the shares
                    db.execute(
                        "INSERT INTO history (user_id, symbol, price, shares, total, activity, date) VALUES (?, ?, ?, ?, ?, ?, ?)",
                        session["user_id"],
                        quote["symbol"],
                        quote["price"],
                        shares,
                        quote["price"] * int(shares),
                        "BOUGHT",
                        datetime.now(),
                    )

                    # Check if the user already have some shares of this stock
                    stock_owned = db.execute(
                        "SELECT * FROM stocks WHERE symbol=? AND user_id=?;",
                        quote["symbol"],
                        session["user_id"],
                    )
                    if stock_owned:
                        new_shares = stock_owned[0]["shares"] + int(shares)
                        db.execute(
                            "UPDATE stocks SET shares=? WHERE user_id=?;",
                            new_shares,
                            session["user_id"],
                        )
                    else:
                        db.execute(
                            "INSERT INTO stocks (user_id, name, symbol, shares) VALUES (?, ?, ?, ?);",
                            session["user_id"],
                            quote["name"],
                            quote["symbol"],
                            shares,
                        )

                    new_balance = cash - (int(shares) * quote["price"])

                    # Update the amount of cash of the user
                    db.execute(
                        "UPDATE users SET cash=? WHERE id=?;",
                        new_balance,
                        session["user_id"],
                    )

                    return redirect("/")


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""

    # Querying the list of transactions
    transactions = db.execute(
        "SELECT * FROM history WHERE user_id=?;", session["user_id"]
    )

    # Formatting the data into USD format
    for transaction in transactions:
        transaction["price"] = usd(transaction["price"])
        transaction["total"] = usd(transaction["total"])

    return render_template("history.html", transactions=transactions)


@app.route("/login", methods=["GET", "POST"])
def login():
    """Log user in"""

    # Forget any user_id
    session.clear()

    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("must provide username", 403)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("must provide password", 403)

        # Query database for username
        rows = db.execute(
            "SELECT * FROM users WHERE username = ?", request.form.get("username")
        )

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(
            rows[0]["hash"], request.form.get("password")
        ):
            return apology("invalid username and/or password", 403)

        # Remember which user has logged in
        session["user_id"] = rows[0]["id"]

        # Redirect user to home page
        return redirect("/")

    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("login.html")


@app.route("/logout")
def logout():
    """Log user out"""

    # Forget any user_id
    session.clear()

    # Redirect user to login form
    return redirect("/")


@app.route("/quote", methods=["GET", "POST"])
@login_required
def quote():
    """Get stock quote."""
    if request.method == "GET":
        return render_template("quote.html")
    else:
        # Check the user input
        if request.form.get("symbol"):
            symbol = request.form.get("symbol")

            # Check if the symbol is valid
            quotes = lookup(symbol)
            if not quotes:
                return apology("must enter a valid symbol", 400)
            else:
                quotes["price"] = usd(quotes["price"])
                return render_template("quoted.html", quotes=quotes)
        else:
            return apology("must enter a valid symbol", 400)


@app.route("/change_password", methods=["GET", "POST"])
@login_required
def change_password():
    """Change user's password"""
    if request.method == "GET":
        return render_template("password.html")
    else:
        # Check the user's input
        old_password = request.form.get("old_password")
        new_password = request.form.get("new_password")
        confirmation = request.form.get("confirmation")

        if (not old_password) or (not new_password) or (not confirmation):
            return apology("must fill all empty fields", 400)
        else:
            # Check the user's old password to confirm identity
            rows = db.execute("SELECT hash FROM users WHERE id=?;", session["user_id"])

            if not check_password_hash(rows[0]["hash"], old_password):
                return apology("Old password incorrect", 400)
            else:
                # Check if the new password and confirmation matches
                if new_password != confirmation:
                    return apology("new password must match confirmation", 400)
                else:
                    # Update new password in database
                    password_hash = generate_password_hash(new_password)
                    db.execute(
                        "UPDATE users SET hash=? WHERE id=?;",
                        password_hash,
                        session["user_id"],
                    )

                    return redirect("/")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""

    # Checking which method was sent to the server
    if request.method == "GET":
        return render_template("register.html")
    else:
        # Checking the user inputs
        if not request.form.get("username"):
            return apology("must provide an username", 400)
        elif not request.form.get("password"):
            return apology("must provide a password", 400)
        elif not request.form.get("confirmation"):
            return apology("must confirm password", 400)
        else:
            username = request.form.get("username")
            password = request.form.get("password")
            confirmation = request.form.get("confirmation")

            # Checking if the password matches the confirmation
            if password != confirmation:
                return apology("password must match confirmation", 400)

            # Checking if the username is available
            if db.execute("SELECT * FROM users WHERE username=?;", username):
                return apology("Username not available", 400)

            # Generating the hash of the user's password
            hash = generate_password_hash(password)

            # Adding the user's account to the database
            db.execute(
                "INSERT INTO users (username, hash) VALUES (?, ?);", username, hash
            )

            # Redirecting to login page
            return redirect("/")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""

    if request.method == "GET":
        # Query the available stock for sell for the user
        stocks = db.execute(
            "SELECT symbol FROM stocks WHERE user_id=?;", session["user_id"]
        )
        return render_template("sell.html", stocks=stocks)
    else:
        # Check the user's input
        symbol = request.form.get("symbol")
        shares = request.form.get("shares")

        if not symbol:
            return apology("must enter a symbol", 400)
        elif (not shares) or (int(shares) < 0) or (not shares.isnumeric()):
            return apology("must enter a positive number", 400)
        else:
            shares = int(shares)
            quote = lookup(symbol)
            if not quote:
                return apology("must enter a valid symbol", 400)
            else:
                # Check if the user own any shares of that stock
                shares_owned = db.execute(
                    "SELECT shares FROM stocks WHERE user_id=? AND symbol=?;",
                    session["user_id"],
                    quote["symbol"],
                )

                if not shares_owned:
                    return apology("You do not own any shares of that stock", 400)
                elif shares_owned[0]["shares"] < shares:
                    return apology("You do not own that many shares of that stock", 400)
                else:
                    # Sell the shares
                    new_shares = shares_owned[0]["shares"] - shares
                    if new_shares == 0:
                        # Cleaning the database if the user have no more shares
                        db.execute(
                            "DELETE FROM stocks WHERE user_id=? AND symbol=?;",
                            session["user_id"],
                            quote["symbol"],
                        )
                    else:
                        db.execute(
                            "UPDATE stocks SET shares=? WHERE user_id=? AND symbol=?;",
                            new_shares,
                            session["user_id"],
                            quote["symbol"],
                        )

                    # Saving the transaction into history
                    db.execute(
                        "INSERT INTO history (user_id, symbol, price, shares, total, activity, date) VALUES (?, ?, ?, ?, ?, ?, ?)",
                        session["user_id"],
                        quote["symbol"],
                        quote["price"],
                        shares,
                        quote["price"] * int(shares),
                        "SOLD",
                        datetime.now(),
                    )

                    # Querying the cash available for the user
                    cash = db.execute(
                        "SELECT cash FROM users WHERE id=?;", session["user_id"]
                    )[0]["cash"]

                    # Update the user's cash disposal
                    new_balance = cash + (shares * quote["price"])
                    db.execute(
                        "UPDATE users SET cash=? WHERE id=?;",
                        new_balance,
                        session["user_id"],
                    )

                    return redirect("/")


@app.route("/add_cash", methods=["GET", "POST"])
@login_required
def add_cash():
    """Add additional cash to the account"""
    if request.method == "GET":
        return render_template("cash.html")
    else:
        # Check the user input
        amount = request.form.get("cash")
        if not amount:
            return apology("must enter a valid amount of cash", 400)
        elif not amount.isnumeric():
            return apology("must enter a valid amount of cash", 400)
        elif int(amount) <= 0:
            return apology("must enter a valid amount of cash", 400)
        else:
            amount = int(amount)
            # Query the amount available in the database
            cash_available = db.execute(
                "SELECT cash FROM users WHERE id=?;", session["user_id"]
            )[0]["cash"]

            # Add the amount to the database
            db.execute(
                "UPDATE users SET cash=? WHERE id=?;",
                cash_available + amount,
                session["user_id"],
            )

            return redirect("/")

