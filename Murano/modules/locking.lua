admin_psk = "<It's a secret>"

request_inspector = {}
email_inspector = {}
 
function request_inspector.has_auth(request_headers)
-- Look for the psk
  if request_headers['x-admin-key'] ~= nil then
    -- Check if the key is good.
    if request_headers['x-admin-key'] == admin_psk then
      return true , "the user is properly authenticated"
    else
      return false, "The authentication key provided isn't right, check yourself."
    end
  else
    return false, "Missing authentication header."
  end
end