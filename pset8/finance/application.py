import os

from cs50 import SQL
from flask import Flask, flash, jsonify, redirect, render_template, request, session
from flask_session import Session
from tempfile import mkdtemp
from werkzeug.exceptions import default_exceptions, HTTPException, InternalServerError
from werkzeug.security import check_password_hash, generate_password_hash

from helpers import apology, login_required, lookup, usd

# Configure application
app = Flask(__name__)

# Ensure templates are auto-reloaded
app.config["TEMPLATES_AUTO_RELOAD"] = True

# Ensure responses aren't cached
@app.after_request
def after_request(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Expires"] = 0
    response.headers["Pragma"] = "no-cache"
    return response

# Custom filter
app.jinja_env.filters["usd"] = usd

# Configure session to use filesystem (instead of signed cookies)
app.config["SESSION_FILE_DIR"] = mkdtemp()
app.config["SESSION_PERMANENT"] = False
app.config["SESSION_TYPE"] = "filesystem"
Session(app)

# Configure CS50 Library to use SQLite database
db = SQL("sqlite:///finance.db")

# Make sure API key is set
if not os.environ.get("API_KEY"):
    raise RuntimeError("API_KEY not set")


@app.route("/")
@login_required
def index():
    """Show portfolio of stocks"""
    
    # Get stocks from the databae grouped by symbol
    stocks = db.execute("SELECT symbol,SUM(shares) FROM transactions WHERE user_id  = :user_id  GROUP BY symbol ",
        user_id=session["user_id"])

    # Initilize total money
    total = 0
    
    for stock in stocks:
        # Get stock name
        stock["name"] = lookup(stock["symbol"])["name"]
        # Get current stock price
        stock["price"] = lookup(stock["symbol"])["price"]
        # Get total stock value
        stock["total"] = usd(stock['price'] * stock['SUM(shares)'])
        # Get total stocks value
        total += stock['price'] * stock['SUM(shares)']
        
    # Get cash left from the database
    rows = db.execute("SELECT cash FROM users WHERE id = :user_id",
        user_id=session["user_id"])
        
    cash = rows[0]["cash"]
    
    # Get total money in stocks + cash
    total += cash
    
    return render_template("index.html", cash=usd(cash), stocks=stocks, total=usd(total))


@app.route("/buy", methods=["GET", "POST"])
@login_required
def buy():
    """Buy shares of stock"""
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
            
        quote = lookup(request.form.get("symbol"))
        
        # Ensure a valid symbol was submitted
        if not quote:
            return apology("invalid symbol", 400)
            
        # Ensure shares were submitted
        if not request.form.get("shares"):
            return apology("missing shares", 400)
        
        # Check if shares was a positive integer
        try:
            shares = int(request.form.get("shares"))
        except:
            return apology("shares must be a positive integer", 400)
            
        # Check if number of shares less than or equals 0
        if shares <= 0:
            return apology("can't buy less than or 0 shares", 400)
            
        # Check if the user has enough cash to afford these shares
        rows = db.execute("SELECT cash FROM users WHERE id = :user_id",
        user_id=session["user_id"])
        
        cash = rows[0]["cash"]
        price = quote["price"]
        totalPrice = price * shares

        
        if totalPrice > cash:
            return apology("you don't have enough cash", 400)
            
        # Add stock to user's portifolio
        db.execute("UPDATE users SET cash = cash - :price WHERE id = :user_id", price=totalPrice, user_id=session["user_id"])
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES(:user_id, :symbol, :shares, :price)",
                   user_id=session["user_id"],
                   symbol=quote["symbol"],
                   shares=shares,
                   price=price)

        
        # Display a flash message
        flash("Shares Bought Successfully!")
        
        return redirect("/")
        
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("buy.html")


@app.route("/check", methods=["GET"])
def check():
    """Return true if username available, else false, in JSONJson format"""
    
    # Get username
    username = request.args["username"]
    
    # Check if the user did not submit a username or it's in the database
    
    if len(username) == 0 or db.execute("SELECT username FROM users WHERE username = :username",
    username=username):
        return jsonify(False)
        
    # Return true if there's no problem
    return jsonify(True)
    


@app.route("/history")
@login_required
def history():
    """Show history of transactions"""
    
    # Get transactions from the database
    transactions = db.execute("SELECT * FROM transactions WHERE user_id = :user_id",
        user_id=session["user_id"])
        
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
        rows = db.execute("SELECT * FROM users WHERE username = :username",
                          username=request.form.get("username"))

        # Ensure username exists and password is correct
        if len(rows) != 1 or not check_password_hash(rows[0]["hash"], request.form.get("password")):
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
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":
        
        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
            
        quote = lookup(request.form.get("symbol"))
        
        # Ensure a valid symbol was submitted
        if not quote:
            return apology("invalid symbol", 400)
            
        return render_template("quoted.html", name=quote["name"], symbol=quote["symbol"], price=quote["price"])
        
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("quote.html")


@app.route("/register", methods=["GET", "POST"])
def register():
    """Register user"""
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure username was submitted
        if not request.form.get("username"):
            return apology("missing username", 400)

        # Ensure password was submitted
        elif not request.form.get("password"):
            return apology("missing password", 400)
            
        # Ensure password confirmation was submitted
        elif not request.form.get("confirmation"):
            return apology("missing password confirmation", 400)
            
        # Ensure password matches password confirmation was submitted
        elif not request.form.get("password") == request.form.get("confirmation"):
            return apology("password mismatch", 400)
            
         # Add user to the database
        username = request.form.get("username")
        hash = generate_password_hash(request.form.get("password"))
        
        new_user_id = db.execute("INSERT INTO users (username, hash) VALUES (:username, :hash)",
                              username=username, hash=hash)
                              
        if not new_user_id:
            return apology("username already exists")
            
        # login user automatically
        session["user_id"] = new_user_id
        
        # Display a flash message
        flash("Registered Successfully!")
    
        # Redirect user to home page
        return redirect("/")
            
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("register.html")


@app.route("/sell", methods=["GET", "POST"])
@login_required
def sell():
    """Sell shares of stock"""
    
    # Get stocks from the databae grouped by symbol
    stocks = db.execute("SELECT symbol,SUM(shares) FROM transactions WHERE user_id  = :user_id  GROUP BY symbol ",
    user_id=session["user_id"])
    
    # User reached route via POST (as by submitting a form via POST)
    if request.method == "POST":

        # Ensure symbol was submitted
        if not request.form.get("symbol"):
            return apology("missing symbol", 400)
            
        # Ensure shares were submitted
        if not request.form.get("shares"):
            return apology("missing shares", 400)
            
        # Ensure user has the nubers of stocks submitted
        for stock in stocks:
            if stock['symbol'] == request.form.get("symbol"):
                if int(request.form.get("shares")) > stock['SUM(shares)']:
                    return apology("you dont have enough shares", 400)
                break
            
        # Update user's portifolio
        quote = lookup(request.form.get("symbol"))

        rows = db.execute("SELECT cash FROM users WHERE id = :user_id",
        user_id=session["user_id"])
        
        cash = rows[0]["cash"]
        price = quote["price"]
        shares = int(request.form.get("shares"))
        totalPrice = price * shares
        
        db.execute("UPDATE users SET cash = cash + :price WHERE id = :user_id",
        price=totalPrice,
        user_id=session["user_id"])
        
        db.execute("INSERT INTO transactions (user_id, symbol, shares, price) VALUES(:user_id, :symbol, :shares, :price)",
                   user_id=session["user_id"],
                   symbol=quote["symbol"],
                   shares=-shares,
                   price=price)
        
        # Display a flash message
        flash("Sold Successfully!")
    
        # Redirect user to home page
        return redirect("/")
            
    # User reached route via GET (as by clicking a link or via redirect)
    else:
        return render_template("sell.html", stocks=stocks)


def errorhandler(e):
    """Handle error"""
    if not isinstance(e, HTTPException):
        e = InternalServerError()
    return apology(e.name, e.code)


# Listen for errors
for code in default_exceptions:
    app.errorhandler(code)(errorhandler)
